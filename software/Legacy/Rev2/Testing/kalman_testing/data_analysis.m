clear; clf;
data=  readtable("elevator_test_4_19.csv")



%%plot all data
figure(1)
clf
hold on
plot(data.TimeSinceLaunch_ms_, data.Altitude_m_, "Linewidth", 2)
plot(data.TimeSinceLaunch_ms_, data.KalmanAltitude, "--")
legend("Raw", "Kalman")
title("Kalman Altitude")

figure(20)
clf
plot(data.TimeSinceLaunch_ms_, data.GlobalZVel, "Linewidth", 2)
title("Kalman Global Z Vel")

figure(30)
clf
plot(data.TimeSinceLaunch_ms_, data.KalmanGlobalZAccel, "Linewidth", 2)
title("Kalman Global Z Acc")

figure(40)
clf
plot(data.TimeSinceLaunch_ms_, data.YAcceleration_m_s_, "Linewidth", 2)
title("Raw Z Acc")

vel = [0];
for i = 2:length(data.KalmanGlobalZAccel)
    vel(end+1) = vel(end) + data.YAcceleration_m_s_(i) * ((data.TimeSinceLaunch_ms_(i) - data.TimeSinceLaunch_ms_(i-1)) / 1000);
end
figure(50)
clf
plot(data.TimeSinceLaunch_ms_, vel)
title("Integrated Vel")


figure(60)
clf
hold on
plot(data.TimeSinceLaunch_ms_, data.XOrientation)
plot(data.TimeSinceLaunch_ms_, data.YOrientation)
plot(data.TimeSinceLaunch_ms_, data.ZOrientation)
legend("X", "Y", "Z")
title("Orint.")