function ex6_2
global  ax  changed mov recording view_fig axis_on xyzbuttons
    recording=0; % no one pressed record-button

    objlist = {'Ellipsoid','Cylinder', 'Box', 'Egg'};
    changed = 1;
    knappkonsol = figure;
    xyzheight = 0.8; %height of the xyz buttons
    height = 0.5;
    axis_on = 0; %whether the axis are shown in the figure, as red=x green=y blue=z

    C = imread('wall_tileable.jpg', 'jpg');%wall texture
    C2 = imread('axis7_.jpg', 'jpg');

    t= timer('TimerFcn', @myupdate,...
        'startdelay', 0,...
        'Executionmode', 'Fixedrate',...
        'Period', 0.05, 'TasksToExecute', Inf,...
        'BusyMode', 'drop');

    set(knappkonsol, 'name', 'GUI',...
        'NumberTitle', 'Off',...
        'Menubar', 'None',...%replace the usual menu
        'Units', 'Centimeters',...
        'DeleteFcn', @cleanup,...
        'Position',  [10 3 10 7]);

        axwidth=7;
        axheight=7;
        
    Axis = uicontrol (...
        'Style', 'pushbutton',...
        'Units', 'centimeters',...
        'Position', [3 0 axwidth axheight],...
        'CData', C2,...    
        'TooltipString', 'highlights axis in figure',...
        'Callback', @axisCB);
    
        xaa=5;
        
    xrotation = uicontrol (...
        'Style', 'pushbutton',...
        'Units', 'centimeters',...
        'Position', [xaa+2.4 2.2 0.5 xyzheight],...
        'String', 'X',...
        'TooltipString', 'Changes rotation axis',...
        'Callback', @xrotCB);

        %green axis
    yrotation = uicontrol (...
        'Style', 'pushbutton',...
        'Units', 'centimeters',...
        'Position', [xaa 2.2 0.5 xyzheight],...
        'String', 'Y',...
        'TooltipString', 'Changes rotation axis',...
        'Callback', @yrotCB);

        %blue axis
    zrotation = uicontrol (...
        'Style', 'pushbutton',...
        'Units', 'centimeters',...
        'Position', [xaa+1.3  height+3 0.5 xyzheight],...
        'String', 'Z',...
        'TooltipString', 'Changes rotation axis',...
        'Callback', @zrotCB);
        
        xyzbuttons = {xrotation,yrotation,zrotation};
        %figure_object also points to timer, which is needed when we close
        %everything (cleanup must reach timer object to stop and delete it)
        
    switchbutton = uicontrol (...
        'Style', 'togglebutton',...
        'Units', 'centimeters',...
        'Position', [0 0 2.5 0.5],...
        'String', 'Toggle',...
        'Value', 0,...    
        'Callback', @switchCB);

    %set(capbutton, 'userdata', knappkonsol);% to capture video

    quitbutton = uicontrol (...
        'Style', 'pushbutton',...
        'Units', 'centimeters',...
        'Position', [0 0.5 2 0.5],...
        'String', 'Quit',...
        'Callback', @cleanup);

    set(quitbutton, 'UserData', t);

    startbutton = uicontrol (...
        'Style', 'pushbutton',...
        'Units', 'centimeters',...
        'Position', [0 6.5 2 height],...
        'String', 'Start',...
        'Callback', @startCB);
        
    stopbutton = uicontrol (...
        'Style', 'pushbutton',...
        'Units', 'centimeters',...
        'Position', [0 6 2 height],...
        'String', 'Stop',...
        'TooltipString', 'Stops the animation',...
        'Callback', @stopCB);

    set(stopbutton, 'enable', 'off')
    set(startbutton, 'userdata', {t, stopbutton});
    set(stopbutton, 'userdata', {t, startbutton}); 
        

    capturebutton = uicontrol (...
        'Style', 'togglebutton',...
        'Units', 'centimeters',...
        'Position', [0 4 2.5 height],...
        'String', ']capture[',...
        'TooltipString', 'Capture figure',...
        'Value', 0,...    
        'Callback', @capture);

    set(capturebutton, 'userdata', quitbutton); %we must disable quit if we capture, hence the pointer

    view_fig = figure;

    set(view_fig, 'doublebuffer');
    set(view_fig, 'renderer', 'opengl');

    Filem = uimenu(view_fig, 'label', 'File');
    alt(1)=uimenu(Filem, 'label', 'Exit',...
            'callback', @cleanup,...
            'accelerator', 'Q');

    Figs = uimenu(view_fig, 'label', 'Figure');

    alt(1) = uimenu(Figs, 'label', objlist{1},...
            'callback', @loadEllipsoid,...
            'accelerator', 'E');
    alt(2) = uimenu(Figs, 'label', objlist{2},...
            'callback', @loadCylinder,...
            'accelerator', 'T');
    alt(3) = uimenu(Figs, 'label', objlist{3},...
            'callback', @loadCube,...
            'accelerator', 'K');
    alt(4) = uimenu(Figs, 'label', objlist{4},...
            'callback', @loadEgg,...
            'accelerator', 'B');
                
    set(alt(1), 'userdata', t); %point loadellipsoid to timern
    set(alt(2), 'userdata', t);
    set(alt(3), 'userdata', t);
    set(alt(4), 'userdata', t);

    set(view_fig, 'name', 'GUI',...
        'NumberTitle', 'Off',...
        'Menubar', 'None',... %replace the usual menu
        'Units', 'Centimeters',...
    'DeleteFcn', @cleanup,...
        'Position',  [10 8 15 15]);

    %rotation axis text
    txt = text(2.7,1.2,4,'?', 'color', [1,1,1],'FontSize',20,'Fontweight','bold');

    [x, y, z]   = ellipsoid(0, 0, 0, 1, 1, 0.3, 30);
    ax = 1;xt = x;yt = y;zt = z;

    axis off

    n = 3;
    [X, Y] = meshgrid(linspace(-1,1,n));
    hold on
    wallx = warp(X, Y, X*0-1.02, C);
    hold on
    wally = warp(X, Y*0-1.02, Y, C);
    hold on
    wallt = warp(X*0-1.02, Y, X, C);
    %no need to update walls
    shadowc = [0,0,0];
    alpha = 0.6;

    indices = convhull(yt, zt);%x
    shadowX = fill3(xt(indices)'*0-1, yt(indices)', zt(indices)', shadowc,'facealpha', alpha, 'edgecolor', 'none');
    hold on
    indices = convhull(xt, zt);%y
    shadowY = fill3(xt(indices)', yt(indices)'*0-1, zt(indices)', shadowc,'facealpha', alpha, 'edgecolor', 'none');
    hold on
    indices = convhull(xt, yt);%z
    shadowZ = fill3(xt(indices)', yt(indices)', zt(indices)'*0-1, shadowc,'facealpha', alpha, 'edgecolor', 'none');
    hold on 
    axlar = plot3([0, 1.3], [0, 0], [0, 0], '-r', [0, 0], [0, 1.3], [0, 0], '-g', [0, 0], [0, 0], [0, 1.3], '-b', 'linewidth', 3);%axis
    set(axlar,'visible','off');
    hold on
    colormap(summer)
    objekt = surf(xt, yt, zt, 'edgecolor', 'none');

    set(t, 'UserData', {objekt, shadowX, shadowY, shadowZ, axlar, txt, knappkonsol, view_fig});

    set(view_fig, 'UserData', t)
    stop(t);

    axis([-1 1 -1 1 -1 1 -1 1]);
    view([1,-1,1]);


    function loadEllipsoid(knappar, event)
        t = get(knappar, 'userdata');
        ud = get(t, 'userdata');
        h = ud{1};
        [x,y,z] = ellipsoid(0, 0, 0, 1, 1, 0.3, 30);
        set(h, 'xdata', x, 'ydata', y, 'zdata', z);
    return

    %cube
    %(ref.http://www.nbb.cornell.edu/neurobio/land/PROJECTS/Hierarchy/superquadric.m)
    %hold off; ex=0.1; [X, Y, Z] = sphere(30); surf(abs(X).^ex.*sign(X), abs(Y).^ex.*sign(Y), abs(Z).^ex.*sign(Z) ); axis square

    function loadCylinder(knappar, event)
        t = get(knappar, 'userdata');
        ud = get(t, 'userdata');
        h = ud{1};
        n=30;
        phi = linspace(0,2*pi,n)';
        z = zeros(n,1); o = z+1;s=sin(phi); c=cos(phi);
        x=[-o -o o o]; y=[z c c z]; z=[z s s z];
        changed=1;
    set(h, 'xdata', x/2, 'ydata', y/2, 'zdata', z/2);
    return

    function loadCube(knappar, event)
        t = get(knappar, 'userdata');
        ud = get(t, 'userdata');
        h = ud{1};
        [x, y, z] = sphere(20);
        ex=0.1;
        set(h, 'xdata', abs(x).^ex.*sign(x)/2, 'ydata', abs(y).^ex.*sign(y)/2, 'zdata', abs(z).^ex.*sign(z)/2);
    return

    function loadEgg(knappar, event)
        t = get(knappar, 'userdata');
        ud = get(t, 'userdata');
        h = ud{1};
        [x, y, z] = sphere(20);
        z=z.*((z>0)+1);
    
        set(h, 'xdata', x/2, 'ydata', y/2, 'zdata', z/2);
    return

    % switches rotational axis
    function switchCB(handle, event)
        global ax changed
        ax = mod(ax+1, 3)+1; %ax<-[0,2]
        changed = 1;
    return

    function cleanup(button, event)
        t = get(button, 'UserData');
        ud = get(t, 'UserData')
        stop(t) 
        delete(t)
    
        if(size(ud,2)==6)
            disp('already deleted windows')
        elseif (size(ud,2)==8)
        
            delete(ud{7})
            delete(ud{8})
        else
        disp('you seem to already have destroyed the windows')
        end

        clear t
    return


    function myupdate(obj, event)
        global ax theta changed mov recording view_fig axis_on xyzbuttons

        ud = get(obj, 'UserData');%obj eller 'handle'... 
        h = ud{1};
        shadowX = ud{2};
        shadowY = ud{3};
        shadowZ = ud{4};
        axlar = ud{5};
        txt = ud{6};

        x = get(h, 'xdata');
        y = get(h, 'ydata');
        z = get(h, 'zdata');

        %v = ud{2}; %[axis, rotation, speed]the rotation
        angle = 0.08;
        s = sin(angle);
        c = cos(angle);

        %Rotations
        if (ax==1)%x
                xt = x;
                yt = c*y - s*z;
                zt = s*y + c*z;
        end

        if (ax==2)%y
                xt = c*x+s*z;
                yt = y;
                zt = -s*x+c*z;
        end

        if (ax==3)%z
                xt = c*x - s*y;
                yt = s*x + c*y;
                zt = z;
        end

        if (changed) %used to update the axis (from start)
            axs = {'X','Y','Z'};
            e = eye(3); 

            for i=1:3
                set(xyzbuttons{i},'backgroundcolor', zeros(3,1));
            end

            set(xyzbuttons{ax},'backgroundcolor', e(:,ax));%gives [1,0,0] if ax==1,...
            xt = x;
            yt = y;
            zt = z;
            %i=zeros(3,1); i(ax)=1;
            if ~axis_on
                set(axlar(:), 'visible','off'); %resetta alla axlar
            end
                
            if axis_on
                set(axlar(:), 'visible','on') %g�r dom synliga
                set(axlar(:), 'LineWidth', 2)%resetta alla axlar
                set(axlar(ax), 'LineWidth', 6)%s�tt den axeln man roterar runt med tjock linjestorlek
            end

            set(txt, 'string', axs{ax});        
                
            %build xdata ydata and zdata and put in the values in here!
            changed=0;%updated for this time
        end


        indices = convhull(yt, zt);%x
        set(shadowX, 'xdata', xt(indices)'*0-1, 'ydata', yt(indices)', 'zdata', zt(indices)');
        indices = convhull(xt, zt);%y
        set(shadowY, 'xdata', xt(indices)', 'ydata', yt(indices)'*0-1, 'zdata', zt(indices)');
        indices = convhull(xt, yt);%z
        set(shadowZ, 'xdata', xt(indices)', 'ydata', yt(indices)', 'zdata', zt(indices)'*0-1);
        
        set(h, 'xdata', xt, 'ydata', yt, 'zdata', zt);
        %the changes to ud has been made and it is updated
        set(obj,  'Userdata', {h, shadowX, shadowY, shadowZ, axlar, txt});%updatera data
        
        if recording
            F = getframe(view_fig);%knappkonsol eller gca f�r knapprad
            mov = addframe(mov, F);
        end
        
    return
        
        %for each tick of the timer, it is capturing a frame

    function capture(handle, event)
    global mov recording;

        capstate={']capture[','[capturing...]'};
        capcolor = {[1, 1, 1],%capture color
                            [1, 0, 0]};%Capturing color
        onoff = {'on','off'};
        quitbutton = get(handle,'userdata');
        set(handle, 'String', capstate{1+ get(handle, 'Value') } );%value<-[0,1]
        set(handle,'Backgroundcolor', capcolor{1 + get(handle, 'Value') });%*get(handle,'backgroundcolor')

        % err = capstate{1+ get(handle, 'Value') }
        
        if get(handle,'value')==1 %capture
                mov = avifile('ex6_test3.avi');
                mov.quality=100;
                %we know we pushed this button and that the file has been opened
                %for writing to
                recording=1;
        else
                mov = close(mov);
                recording=0;%we have stopped recording
                %clear all
                clear mex
                disp( 'capture complete...');
        end

        %onoff{1+get(stopbutton, 'Value')}
        set(quitbutton, 'Enable', onoff{1+get(handle, 'Value')} );% m�ste anv�nda handle f�r att f� fram capture value
    return

    function startCB(this, event)
    global time  stopbutton
        ud=get(this,'userdata');
        t = ud{1};
        stopbutton=ud{2};
        set(this, 'Enable', 'off')
        set(stopbutton, 'Enable', 'on')

        changed=1%first time we push start, we want this, explicitly
        start(t)
    return


    function stopCB(this, event)
        global time startbutton% stopbutton
        ud = get(this, 'userdata');%point at startbutton
        t = ud{1};
        startbutton=ud{2};
        set(startbutton, 'Enable', 'on')
        set(this, 'Enable', 'off')
        %check if capturing, leave inactive if it is on
        stop(t)
    return

    function axisCB(handle,event)
            global axis_on changed
            changed=1;%update this
            axis_on=-axis_on+1;%toggles 'axis on'
    return        

    function xrotCB(handle,event)
    global ax changed
        ax=1; changed=1;
    return

    function yrotCB(handle,event)
    global ax changed
        ax=2; changed=1;
    return

    function zrotCB(handle,event)
    global ax changed
        ax=3; changed=1;
    return
