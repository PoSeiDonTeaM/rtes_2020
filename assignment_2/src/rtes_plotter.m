executionTime = 'executionTime.csv';
inputDuration = 'inputDuration.csv';
jobAliveTime = 'jobAliveTime3.csv';
totalDrift = 'totalDrift.csv';

M = csvread(executionTime);
N = csvread(inputDuration);
O  = csvread(jobAliveTime);
P  = csvread(totalDrift);

t = length(M);
time = zeros(1, t);

max_value = max(M, [], 'all');
min_value = min(M, [], 'all');
median_value = median(M, 'all');
average = mean(M, 'all');
standardDeviation = std(M,median_value, 'all');

fprintf("================================== \n");
fprintf("Execution time\n");
fprintf("================================== \n");
fprintf("Max = %d \n", max_value);
fprintf("Min = %d \n", min_value);
fprintf("Median = %d \n", median_value);
fprintf("Mean = %d \n", average);
fprintf("StandardDeviation = %d \n", standardDeviation);
fprintf("================================== \n\n\n");


    for i = 1:t
        time(i) = i;
    end

figure();
plot(time, M);
title("Execution Time");
xlabel("# of Tasks");
ylabel("Duration (in usec)");
saveas(gcf,'C:\Users\giannis_kotsas\Documents\HMMY\Real-Time Embedded Systems\rtes_2020-master\assignment_2\rpi_results\main1\executionTime.png')

t = length(N);
time = zeros(1, t);
    for i = 1:t
        time(i) = i;
    end
    
max_value = max(N, [], 'all');
min_value = min(N, [], 'all');
median_value = median(N, 'all');
average = mean(N, 'all');
standardDeviation = std(N,median_value, 'all');

fprintf("================================== \n");
fprintf("Input Duration \n");
fprintf("================================== \n");
fprintf("Max = %d \n", max_value);
fprintf("Min = %d \n", min_value);
fprintf("Median = %d \n", median_value);
fprintf("Mean = %d \n", average);
fprintf("StandardDeviation = %d \n", standardDeviation);
fprintf("================================== \n\n\n");

figure();
plot(time, N);
title("Input Duration");
xlabel("# of Tasks");
ylabel("Duration (in usec)");
saveas(gcf,'C:\Users\giannis_kotsas\Documents\HMMY\Real-Time Embedded Systems\rtes_2020-master\assignment_2\rpi_results\main1\inputDuration.png')

t = length(O);
time = zeros(1, t);
    for i = 1:t
        time(i) = i;
    end
    

max_value = max(O, [], 'all');
min_value = min(O, [], 'all');
median_value = median(O, 'all');
average = mean(O, 'all');
standardDeviation = std(O,median_value, 'all');

fprintf("================================== \n");
fprintf("Job Alive Time \n");
fprintf("================================== \n");
fprintf("Max = %d \n", max_value);
fprintf("Min = %d \n", min_value);
fprintf("Median = %d \n", median_value);
fprintf("Mean = %d \n", average);
fprintf("StandardDeviation = %d \n", standardDeviation);
fprintf("================================== \n\n\n");

figure();
plot(time, O);
title("Job Alive Time");
xlabel("# of Tasks");
ylabel("Duration (in usec)");
saveas(gcf,'C:\Users\giannis_kotsas\Documents\HMMY\Real-Time Embedded Systems\rtes_2020-master\assignment_2\rpi_results\main1\jobAliveTime.png')

t = length(P);
time = zeros(1, t);
    for i = 1:t
        time(i) = i;
    end
    

max_value = max(P, [], 'all');
min_value = min(P, [], 'all');
median_value = median(M, 'all');
average = mean(P, 'all');
standardDeviation = std(P,median_value, 'all');

fprintf("================================== \n");
fprintf("Drifting \n");
fprintf("================================== \n");
fprintf("Max = %d \n", max_value);
fprintf("Min = %d \n", min_value);
fprintf("Median = %d \n", median_value);
fprintf("Mean = %d \n", average);
fprintf("StandardDeviation = %d \n", standardDeviation);
fprintf("================================== \n\n\n");

figure();
plot(time, P);
title("Drifting");
xlabel("# of Tasks");
ylabel("Duration (in usec)");
saveas(gcf,'C:\Users\giannis_kotsas\Documents\HMMY\Real-Time Embedded Systems\rtes_2020-master\assignment_2\rpi_results\main1\drifting.png')
