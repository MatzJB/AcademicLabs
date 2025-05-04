%https://en.wikipedia.org/wiki/Dormand%E2%80%93Prince_method
function r = DOPRI(h)
r = 1 + h + h.^2 / 2 + h.^3 / 6 + h.^4 / 24 + h.^5 / 120 + h.^6 / 600;