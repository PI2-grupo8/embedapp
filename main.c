#include "arduino/tests/comm/rasp_uart.h"
#include "arduino/tests/comm/sensors.h"
#include "arduino/tests/comm/actuators.h"
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define CLOSE_DISTANCE 5
#define FAR_DISTANCE 15
#define NO_WALL_DISTANCE 150
#define SAFE_CURVE_DISTANCE 30
#define CONST_SPEED 6.2

void start_drill(){
    printf("Ready to start drill!\n");
}

void check_wall_distance(){
    float top_sonar_value = 0;
    float bottom_sonar_value = 0;

    top_sonar_value = read_sonar(0);
    bottom_sonar_value = read_sonar(1);

    if((top_sonar_value > CLOSE_DISTANCE && top_sonar_value < FAR_DISTANCE) && (bottom_sonar_value > CLOSE_DISTANCE && bottom_sonar_value < FAR_DISTANCE)){
        direction(0);
    }
    else if((top_sonar_value > FAR_DISTANCE) && (bottom_sonar_value > CLOSE_DISTANCE && bottom_sonar_value < FAR_DISTANCE)){
        direction(2);
    }
    else if((top_sonar_value < CLOSE_DISTANCE) && (bottom_sonar_value > CLOSE_DISTANCE && bottom_sonar_value < FAR_DISTANCE)){
        direction(-2);
    }
    else if((top_sonar_value < CLOSE_DISTANCE) && (bottom_sonar_value < CLOSE_DISTANCE)){
        direction(-1);
    }
    else if((top_sonar_value > CLOSE_DISTANCE && top_sonar_value < FAR_DISTANCE) && (bottom_sonar_value < CLOSE_DISTANCE)){
        direction(2);
    }
    else if((top_sonar_value > FAR_DISTANCE) && (bottom_sonar_value < CLOSE_DISTANCE)){
        direction(3);
    }
    else if((top_sonar_value > FAR_DISTANCE) && (bottom_sonar_value > FAR_DISTANCE)){
        direction(1);
    }
    else if((top_sonar_value > CLOSE_DISTANCE && top_sonar_value < FAR_DISTANCE) && (bottom_sonar_value > FAR_DISTANCE)){
        direction(-2);
    }
    else{
        // printf("S1: %.3f - S2: %.3f\n", top_sonar_value, bottom_sonar_value);
        direction(-3);
    }
}

void make_first_curve(){
    engine(1);

    float top_sonar_value = 0;
    top_sonar_value = read_sonar(0);

    while(top_sonar_value < NO_WALL_DISTANCE){
        check_wall_distance();
        top_sonar_value = read_sonar(0);
    }

    printf("CHEGOU NA CURVA\n");

    direction(0);
    double time = 5 * (double)CLOCKS_PER_SEC;
    clock_t tic = clock();
    clock_t toc = clock();

    while((double)(toc - tic) < time){
        toc = clock();
    }

    direction(3);
    sleep(30);

    printf("INICIANDO WALL FOLLOWER DE NOVO\n");

    int i = 0;
    for(i=0; i<1000;i++){
        check_wall_distance();
    }
    engine(0);
    // while(total_moved < SAFE_CURVE_DISTANCE){
    //     clock_t start = clock(), diff; // Start timer

    //     speed = get_current_speed()    // Get current speed in m/s

    //     diff = clock() - start;        // Stop timer

    //     int millseconds_taken = (diff * 1000 / CLOCKS_PER_SEC)%1000;
    //     total_moved += calculate_distance(timer, millseconds_taken);
    // }

    // top_sonar_value = read_sonar(0);

    // while(top_sonar_value > FAR_DISTANCE){
    //     check_wall_distance();
    //     top_sonar_value = read_sonar(0);
    // }
}

void make_second_curve(){

}

void make_big_curve(){

}

void checks_ended_circuit(int lines_completed, int total_number_lines){
    if(lines_completed < total_number_lines){
        // Circuit completed
        // Stop veicule
        printf("Circuit Ended!\n");
        exit(0);
    }
}

void complete_line(int *lines_completed, int measurement_distance, int total_measurements_per_line){
    // double time = (measurement_distance / CONST_SPEED) * (double)CLOCKS_PER_SEC;
    double time = 100000 * measurement_distance;
    int measurements_make_in_line = 0;
    clock_t tic = clock();
    clock_t toc = clock();

    while(measurements_make_in_line < total_measurements_per_line){
        engine(1);
        while((double)(toc - tic) < time){
            check_wall_distance();
            toc = clock();
        }
        engine(0);
        sleep(5);
        start_drill();
        measurements_make_in_line++;
        tic = toc;
    }

    (*lines_completed)++;
}

void start_navigation(int total_number_lines, int measurement_distance, int total_measurements_per_line){
    int lines_completed = 0;

    while(lines_completed < total_number_lines){
        complete_line(&lines_completed, measurement_distance, total_measurements_per_line);

        checks_ended_circuit(lines_completed, total_number_lines);

        // make_first_curve();
        // make_second_curve();

        // complete_line(&lines_completed, measurement_distance, total_measurements_per_line);

        // checks_ended_circuit(lines_completed, total_number_lines);

        // make_big_curve();
    }
}

int main (int argc, char* argv[]){
    int total_number_lines = 0;
    int total_measurements_per_line = 0;
    int measurement_distance = 0;
    FILE *settings_file = fopen("settings.conf", "r");

    fscanf(settings_file, "%d %d %d", &total_number_lines, &total_measurements_per_line, &measurement_distance);
    // measurement_distance = measurement_distance * 100; // Transformando distancia de m para cm

    char* device = (argc < 2) ? MODEMDEVICE : argv[1];

    uart_init(device);

    // start_navigation(total_number_lines, measurement_distance, total_measurements_per_line);

    make_first_curve();

    fclose(settings_file);

    uart_end();

    return 0;
}
