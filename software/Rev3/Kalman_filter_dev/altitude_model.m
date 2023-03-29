function [T, D, drag_coeff] = altitude_model(dt)
   
    %inital conditions
    p_0 = [0 ; 0 ; 100];
    v_0 = [0 ; 10 ; 300];
    drag_coeff = 0.01;


    %defien state vector
    state = [p_0 ; v_0;  drag_coeff];
    options = odeset('Events', @event_func);
    time = [0:dt:60];

    %solve ODE
    [T, D] = ode45(@rate_func, time, state, options);
    acc = rate_func(T, D');
    D = [D  acc'];
    
    function rate = rate_func(t, state)
        %slice out the position and velocity
        p = state(1:3,:);
        v = state(4:6,:);
        %set the derivatives
        dpdt = v;
        dvdt = acceleration(v);
        num_inputs = size(dpdt);
        num_inputs = num_inputs(2);
        rate = [dpdt ; dvdt ; zeros(1,num_inputs)];

        
    end


    function res = acceleration(v)
        %values for drag equation
        rho = 1.293;
        A = 0.15^2*pi;
        m_rocket = 20;
        f_drag = -0.5 * rho * norm(v)^2 * drag_coeff * A;
        %acceleration due to drag using F=ma
        a_drag = (v / norm(v)) * (f_drag / m_rocket);
        %acceleration due to gravity
        a_grav = [0 ; 0 ; -9.8];
        %combine and return results
        res = [a_grav + a_drag];
    end

    %event function to detect end of flight
    function [value, isterminal, direction] = event_func(t, state)
        value = state(3);
        isterminal = 1;
        direction = -1;
    end

end