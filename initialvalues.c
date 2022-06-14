//////////////////////////////////////////////////////////////
//                      INITIAL VALUES                      //
//////////////////////////////////////////////////////////////
int mode = 0;
int systick_count = 0;
int count = 8;
int turndir = 0;
int turnspeed = 50;
int sensor_selector = 0;
int servo_start = 8;
int servo_stop = 28;

int ir_dist = 0;
int ir_raw = 0;
int us_dist = 0;
int us_raw = 0;
int us_calibration_adjust = 0;
int ir_calibration_adjust = 0;
int calib_tracker = 0;
int us_calib_total = 0;
int ir_calib_total = 0;
int calibrated_flag = 0;
int act_val;
int servoangle;
int sweep_num = 0;

int time_arr[100];
int angle_arr[100];
int ir_dist_arr[100];
int ir_raw_arr[100];
int us_dist_arr[100];
int us_raw_arr[100];
int ir_avg;
int us_avg;
int array_counter = 0;
int samplerate = 2500;
int num = 0;
float x = 0;
float y = 0;

int multicheck = 0;
int newmulti = 0;
