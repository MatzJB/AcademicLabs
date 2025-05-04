
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!                             HPC Lab 4                              !
!               "Parallel Poisson's algorithm using MPI"             !
!                    by Matz Johansson Bergström                     !
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!Potential issues:
!* None
!

!SYNOPSIS:

!The program uses four processes with MPI-calls to solve Poisson's
!equation (with Dirichlets boundary values). The program stores a
!local matrix <my_matrix> for the calculations. <my_matrix> is one
!fourth (just about) version of the super matrix which is (n+2)x(n+2).

!subroutine <worker> is doing one iteration and sending the difference
!between the matrices to calculate a delta value (max difference
!between iterations) using a buffer (to conserve space).

!The MPI calls are broadcast, sendrecv, barrier, send and recv. The
!*-communication uses modulo arithmetic to offset indices and
!send/receive data as 1D arrays.
  
!The while loop is controlled by <tau> and <delta>, the deltas are
!sent from the slaves to the master (stored in an array <deltas>), if
!the deltas are all smaller than tau then the while-loop will
!stop. The while loop is controlled by a variable (proceed) that all
!slaves has a copy of. The variable is changed in a broadcast from the
!master if ( max(deltas)<tau ) is true.


!The memory used by this algorithm: (only counting arrays depending on
!n and necessary for the solution) this is basically the three arrays:

! * my_matrix: (n/2+2)^2
! * my_f: (n/2)^2
! * v (buffer): n/2+2
!
! which is...
!
! (n^2/4 + 2n + 4) + (n^2/4) + (n/2 + 2) <=>
! n^2/2 + 2.5n + 6  <  n^2/2 + 3n + 8 (the lab "limit")

!For obvious reasons <my_facit> is not included in above calculations
!since it is not really part of the algorithm, only for testing.

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


program main
  implicit    none
  
  include    "mpif.h"

  INTEGER   length, source, dest, tag, err
  INTEGER   my_rank !ID of process
  INTEGER   n_procs ! number of processes 

  INTEGER   n, m, k, i, j, t, proceed
  INTEGER   iteration, index

  DOUBLE PRECISION h, x, y, tau
  
  INTEGER   horiz_recv(0:3), vert_recv(0:3) !note: index range
  INTEGER   horizontal_index, vertical_index
  INTEGER   v_offset, h_offset
  INTEGER   debug
  !integer, dimension(4)::progress

  DOUBLE PRECISION, allocatable, dimension(:,:) :: my_matrix, my_facit, my_f
  DOUBLE PRECISION, dimension(4):: deltas
  DOUBLE PRECISION, allocatable, dimension(:) :: v !worker buffer  

!interface for the functions
  interface

     FUNCTION f(x,y)
     implicit none
     DOUBLE PRECISION x, y, f
   end function f

     FUNCTION g(x,y)
     implicit none
     DOUBLE PRECISION x, y, g
   end function g

end interface


  call MPI_Init(err)  ! Start up MPI

  !The rank and the number of processes
  call MPI_Comm_rank(MPI_COMM_WORLD, my_rank, err)
  call MPI_Comm_size(MPI_COMM_WORLD, n_procs, err)


  if(n_procs/=4) then
     if (my_rank==0) then
        print *,'ERROR, use four processes! e.g. "mpirun -n 4 -mca btl tcp,self ./a.out"'
     endif
     call MPI_Finalize(err)  ! Shut down MPI 
     stop
  endif

  tau        = 1E-9 !!!!!!!

  !progress  = (/0, 0, 0, 0/)!progress bar
  vert_recv  = (/1, 0, 3, 2/) !vertical receive IDs (communication)
  horiz_recv = (/2, 3, 0, 1/) !horizontal receive IDs
  iteration  = 0
  debug      = 1!If debug is 1 then we will write to file

  if (my_rank==0 .and. debug==1) then
     print *, 'Debug mode (writing to file)'
  endif

  
!writing to files F<my_rank>.txt (should be able to concat strings...)
  if (debug==1) then
     SELECT CASE (my_rank)
     CASE (0)
        open(unit=0, file="F0.txt", status="replace")
     CASE (1)
        open(unit=1, file="F1.txt", status="replace")
     CASE (2)
        open(unit=2, file="F2.txt", status="replace")
     CASE (3)
        open(unit=3, file="F3.txt", status="replace")
     end SELECT
  endif
  
  if (my_rank==0) then
     print *, '**********************************'
     print *, 'Enter size of matrix (even number < 100), (press enter twice):'
     read (*, '(I3)'), n
     print *, 'Please wait...'
  endif
  
  !broadcast n, each slave allocate their own matrices
  call MPI_BCAST(n, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, err) 

  !Note:All slaves read this
  if (n>100) then
     if (my_rank==0) then
        print *, 'ERROR: The entered dimension is too large, quitting...'   
     endif
     call MPI_Finalize(err)  ! Shut down MPI 
     stop
  endif

 
  m = n/2 + 2
  

  if (mod(n, 2) /= 0) then
     if (my_rank==0) then
        print *, 'Dimension of matrix is not an even number, quitting...'   
     end if
     call MPI_Finalize(err)  ! Shut down MPI 
     stop
  end if

  allocate(my_matrix(m, m), my_f(m-2, m-2), my_facit(m, m), v(m), stat=err) 
    
  if (err/=0) then
     if (my_rank==0) then  
        print *, 'ERROR: Not enough memory, quitting...'   
     endif
     call MPI_Finalize(err)  ! Shut down MPI 
     stop
  endif
    
  do i=1,m
     do j=1,m
        my_matrix(i,j) = 0
        my_facit(i,j)  = 0
     enddo
  enddo
  
   
  h  = 1/dble(n+1)!note: n, since we step in the "super matrix" 
 
  !LOCAL MATRICES <my_matrix>,<my_f>: Divide the super matrix in four
  !smaller ones (mxm). The matrices are built from x,y which are indices
  !for the start of the local matrix (in the super). The my_matrix is
  !built w.r.t m, in a similar way.
  
  x  = mod(my_rank, 2)*(m-2)
  y  = int(my_rank/2)*(m-2)
  
  !For each process we have the local matrix <my_f>. Note that we don't
  !need the "seam" values of the matrix hence we use the indices 1:m-2

  do i=1,m-2
     do j=1,m-2
        my_f(i, j) = f((x+j)*h, 1 - (y+i)*h)
     enddo
  enddo
  

 !call writematrix(my_rank, my_f, m-2) 

  !INITIALIZING BOUNDARY VALUES FOR EACH MATRIX
  !Please note: 
  ! * Index to coordinates uses h and offset with t-1
  
  SELECT CASE (my_rank)
  CASE (0)
     do t=1,m
        my_matrix(t, 1) = g(dble(0), 1 - (t-1)*h) !left
        my_matrix(1, t) = g((t-1)*h, dble(1)) !upper
     enddo
     
  CASE (1)
     do t=1,m
        my_matrix(t, m) = g(dble(1), 1 - (t-1)*h) !right
        my_matrix(1, t) = g((t + m - 3)*h, dble(1)) !upper
     enddo
     
  CASE (2)
     do t=1,m
        my_matrix(t, 1) = g(dble(0), 1 - (t + m - 3)*h) !left
        my_matrix(m, t) = g((t-1)*h, dble(0)) !lower
     enddo
     
  CASE (3)
     do t=1,m
        my_matrix(t, m) = g(dble(1), 1 - (t + m - 3)*h) !right
        my_matrix(m, t) = g((t + m - 3)*h, dble(0)) !lower  
     enddo
  END SELECT
  
  
  !The solution matrix <my_facit>: We only need to calculate 2:m-1 since the
  !"seam" consists either of boundary values or temp values (*-values)
  !for each local matrix.

  SELECT CASE (my_rank)
  CASE (0)  
     do i=2,m-1 
        do j=2,m-1  
           my_facit(i, j) = g((j-1)*h, 1 - (i - 1)*h)
        enddo
     enddo
  CASE (1)
     do i=2,m-1 
        do j=2,m-1  
           my_facit(i, j) = g((j + m - 3)*h, 1 - (i - 1)*h)
        enddo
     enddo
  CASE (2)
     do i=2,m-1
        do j=2,m-1
           my_facit(i,j) = g((j - 1)*h, 1 - (i + m - 3)*h)
        enddo
     enddo
  CASE (3)
     do i=2,m-1
        do j=2,m-1
           my_facit(i,j) = g((j + m - 3)*h, 1-(i + m - 3)*h)
        enddo
     enddo
  END SELECT

  !The communication uses "offset variables".  v_offset means
  !"vertical offset", this is a value: -1 or 1 depending on
  !receiving/sending process (the indexing). h_offset is the same but
  !horizontal offset.
  
  !h_offset is the same but is not depending on modulo (see below).
  !horizontal_index and vertical_index are the indices used to
  !send/receive the *-values.  The sending/receiving calls with
  !indices made up from the above techniques to make the code shorter.
  !vert_recv, horiz_recv are arrays with the ranks of the receiving
  !processes.
  
  v_offset       = 2*mod(my_rank, 2) - 1 !-1 if my_rank is even, 1 otherwise
  
!vertical communication
  vertical_index = mod(my_rank + 1, 2)*(m-3) + 2

!horizontal communication
  if(my_rank<=1) then
     horizontal_index = m-1
  else
     horizontal_index = 2
  endif

  !update matrix with neighbouring *-values
  v_offset = - v_offset

  if (my_rank<=1) then 
     h_offset = 1 
  else 
     h_offset = -1 
  endif

  proceed = 1
  
  do while (proceed == 1)  
     
     call MPI_Barrier(MPI_COMM_WORLD, err)!barrier synch
   
     !horizontal *-values
     call MPI_SendRecv(my_matrix(horizontal_index, :), m, MPI_DOUBLE_PRECISION, &
          horiz_recv(my_rank), 10, my_matrix(horizontal_index + h_offset, :), m, &
          MPI_DOUBLE_PRECISION, horiz_recv(my_rank), 10, &
          MPI_Comm_WORLD, MPI_Status_IGNORE, err)
     
     !vertical *-values
     call MPI_SendRecv(my_matrix(:, vertical_index), m, MPI_DOUBLE_PRECISION, &
          vert_recv(my_rank), 10, my_matrix(:, vertical_index + v_offset), m, &
          MPI_DOUBLE_PRECISION, vert_recv(my_rank), 10, &
          MPI_Comm_WORLD, MPI_Status_IGNORE, err)
        

     if (my_rank==0) then
   
        iteration = iteration + 1        
        call work(my_rank, my_matrix, my_f, v, m, h)
        
        do index=1,4
           !receive deltas for each process and put in array (no order)
           call MPI_Recv(deltas(index), 1, &
                MPI_DOUBLE_PRECISION, MPI_ANY_SOURCE, &
                MPI_ANY_TAG,  MPI_COMM_WORLD, &
                MPI_STATUS_IGNORE, err)
        end do
                
        !The criterion to stop the iterations, if maximum of array is
        !< tau => (every element in array < tau):
        if (maxval(deltas) < tau) then
           proceed = 0
           print *, '...reached the termination criterion (delta<tau).'
        end if
        
        !broadcast the variable <proceed>
        call MPI_BCAST(proceed, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, err) 
     else
        call work(my_rank, my_matrix, my_f, v, m, h)
        call MPI_BCAST(proceed, 1, MPI_INTEGER, 0, MPI_COMM_WORLD, err)
     end if
     
  end do!While
  

  !The result:  
  if (my_rank==0) then
     write (*,'("Iteration #: ", I6, ", delta = ", ES16.10)'), iteration, maxval(deltas)         
  end if
  
  call printerror(my_rank, my_matrix, my_facit, m)
  
  !write the approximation and the exact(!) solution to files
  if (debug==1) then  
     write (my_rank, *), '       (((===== U_approx =====)))'
     call writematrix(my_rank, my_matrix, m) 
     write (my_rank, *), '       (((===== U_exact =====)))'
     call writematrix(my_rank, my_facit, m) 
  end if

  call MPI_Finalize(err) ! Shut down MPI 
  deallocate(my_matrix, my_f, my_facit, v)
  
  close(my_rank)!close files
  
end program main


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!Print the max error between my_matrix and the exact solution
!(my_facit)

subroutine printerror(my_rank, matrix, exact, m)
  INTEGER, INTENT(IN) :: m
  double precision, INTENT(OUT), DIMENSION(m, m):: matrix, exact
  double precision error
  
  integer my_rank

  error = 0

  do i=2,m-1
     do j=2,m-1
        error = max(error, abs(matrix(i,j) - exact(i,j)))
     enddo
  enddo
  
  write (*,'("rank:", I2, ", error= ", ES16.10)'), my_rank, error
  
end subroutine printerror

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!print matrix elements in file indicated by my_rank
subroutine writematrix(my_rank, matrix, n)
  
  INTEGER, INTENT(IN) :: n
  double precision, INTENT(OUT), DIMENSION(n, n):: matrix
  integer  i, j
  integer  my_rank

  do i=1,n
     do j=1,n
        write (my_rank, '(f10.4)', advance="no"), matrix(i, j) 
     enddo
     write (my_rank, *), ''
  enddo

  write (my_rank, *), ''
  
end subroutine writematrix


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!Worker does one iteration, sends delta to master and writes the
!matrix to the file F<my_rank>.txt

!U and f is the local matrices used in the iteration, v is used as a
!buffer, the buffer is allocated for each iteration

subroutine work(my_rank, U, f, v, m, h)

  implicit none
  
  include    "mpif.h"

  INTEGER err, my_rank
  INTEGER end, j, k

  INTEGER, INTENT(IN) :: m
  DOUBLE PRECISION, INTENT(OUT), DIMENSION(m, m) :: U
  DOUBLE PRECISION, INTENT(OUT), DIMENSION(m-2, m-2) :: f
  DOUBLE PRECISION, INTENT(OUT), DIMENSION(m) :: v
  DOUBLE PRECISION tmp, delta, h
    

  delta  = dble(0)
  end    = m
    
  v      = U(1, :)
  tmp    = U(2, 1)
    
  do j=2, end-1, 1
     do k = 2, end-1, 1
        !update U(j,k) with v(k)
        U(j-1, k-1) = v(k-1)
        v(k-1)      = tmp
        tmp = 0.25*( U(j-1, k) + U(j+1, k) +&
                     U(j, k-1) + U(j, k+1) - & 
                     h**2 * f(j-1, k-1) )!j,k but offset for storage

     end do !for 2
     
     U(j-1, end-1) = v(end-1) !write to U before you save to buffer
     v(end-1)    = tmp !save the last calculation in buffer
     tmp         = U(j+1, 1) !next row first element is the tmp 
     v(1)        = U(j, 1)   
    
     delta = max(delta, maxval( abs(v(2:end-1) - U(j, 2:end-1)) ))
  end do !for 1
  
  U(end-1, 2:end-1) = v(2:end-1) 
  !flush the buffer to the last computed row
  
  call MPI_Send(delta, 1, MPI_DOUBLE_PRECISION, 0, 1, MPI_COMM_WORLD, err)
  
end subroutine work


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


DOUBLE PRECISION function f(x, y)
  DOUBLE PRECISION x, y
  f = 2*(cos(x + y) - (1 + x)*sin(x + y))
  return
end function f


DOUBLE PRECISION function g(x, y)
  DOUBLE PRECISION x, y
  g = (1 + x)*sin(x + y)
  return
end function g
