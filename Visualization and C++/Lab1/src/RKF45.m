% Runge-Kutta-Fehlberg
% https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta%E2%80%93Fehlberg_method
function r = RKF45(h)
r = 1 + h + h.^2 / 2 + h.^3 / 6 + h.^4 / 24 + h.^5 / 104;
