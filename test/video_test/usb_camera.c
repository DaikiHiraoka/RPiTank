/*
  コンパイル用コマンド
  gcc -o video_test video_test.c -I/usr/local/include/opencv -lpthread -ldl -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -lm -lrt
*/

#include <stdio.h>
#include <time.h>
#include <cv.h>
#include <highgui.h>

/* fps合わせ */
// #include <pthread.h>
#define NSEC_PER_SEC  1000000000 // = 1 [sec]
#define INTERVAL  10000000 // [ns]
#define DELTA_T 0.2
/*
extern int clock_nanosleep(clockid_t __clock_id, int __flags,
                           __const struct timespec *__req,
                           struct timespec *__rem);
extern int clock_gettime(clockid_t clk_id, struct timespec *res);
*/
/* the struct timespec consists of nanoseconds
 * and seconds. if the nanoseconds are getting
 * bigger than 1000000000 (= 1 second) the
 * variable containing seconds has to be
 * incremented and the nanoseconds decremented
 * by 1000000000.
 */
static inline void tsnorm(struct timespec *ts)
{
  while (ts->tv_nsec >= NSEC_PER_SEC) {
    ts->tv_nsec -= NSEC_PER_SEC;
    ts->tv_sec++;
  }
  /* printf("tv_sec: %d\ntv_nsec: %ld\n", ts->tv_sec, ts->tv_nsec); */
}


int Camera_Init(CvCapture *capture, double wide, double height){
  if (capture==NULL) {
    puts("Camera not found.\n");
    return -1;
  }

  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, wide);
  cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, height);

  return 0;
}

int Camera_End(CvCapture *capture, CvVideoWriter *vw, int video_run){
  /* 解放 */
  if(video_run == 1) {
    cvReleaseVideoWriter(&vw);
    video_run = 0;
  }
  cvReleaseCapture(&capture);
  return 0;
}

double Calc_FPS(int str, double f) {
  double fps = 0.0;
  int end;
  double tms;

  /* fps計算用 */
  end = cvGetTickCount() - str;
  tms = end / (f * 1000.0);
  fps = 1000.0 / tms;
  return fps;
}


int main (int argc, char **argv)
{
  struct timespec interT;
  int interval = DELTA_T * 1000000000; //[ns]
  clock_gettime(0, &interT);

  double w = 640, h = 360;
  // double w = 1280, h = 720;
  int key = 0;
  int end_flag = 1;
  int video_run = 0;
  time_t t;
  int str, end;
  double fps, f, tms; /* f: 周波数, tms: 時間[ms] */
  char file_name[128];
  IplImage *frame;
  CvVideoWriter *vw; /* ビデオライター */
  CvCapture *capture = cvCreateCameraCapture(-1);

  if(Camera_Init(capture, w, h) == -1) {
    puts("Camara Init ERROR !!!");
    return 0;
  }
  // ウィンドウを作成する
  char *windowName = "camera";
  cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);

  /* fps計算用 */
  //str = cvGetTickCount();
  f = cvGetTickFrequency();

  while (end_flag) {
    /* fps計算用 */
    str = cvGetTickCount();
    frame = cvQueryFrame(capture);

    /* 表示フレーム更新 */
    cvShowImage(windowName, frame);

    if(video_run == 1) {
      /* fps分のフレームが取得できていないことがある */
      /* フレーム足りない->動画早い */
      cvWriteFrame(vw, frame);
    }

    printf("fps: %lf\n", Calc_FPS(str, f)); /* fpsの表示 現在8fpsくらい*/

    /* キー入力待ち */
    key = cvWaitKey(1);
    /* リモートとローカルでキーコードが変わったので対応 */
    if(key > 1000000) {
      key -= 0x100000;
    }
    /* printf("%d\n", key); */
    switch(key) {
    case 113: /* q */
      end_flag = 0;
      break;
    case 118: /* v */
      if(video_run == 0) {
	t = time(NULL);
	strftime(file_name, sizeof(file_name), "./avi/%Y_%m_%d_%H_%M_%S.avi", localtime(&t));
	/* ビデオライター設定 */
	vw = cvCreateVideoWriter(/*file_name*/"./avi/test.avi", CV_FOURCC ('M', 'J', 'P', 'G'), 5, cvSize ((int) w, (int) h), 1);
	printf("Video START: %s\n", file_name);
	video_run = 1;
      } else {
	cvReleaseVideoWriter(&vw);
	printf("Video STOP: %s\n", file_name);
	video_run = 0;
      }
      break;
    }
    /* 制御周期まで待機 */
    interT.tv_nsec += interval; /*  */
    /* printf("interval:%d\n", interval); */
    /* printf("interT.tv_nsec:%ld\n", interT.tv_nsec); */
    tsnorm(&interT);
    clock_nanosleep(0, TIMER_ABSTIME, &interT, NULL); /* 渡した時間まで待つ */
  }
  /* 解放 */
  /* if(video_run == 1) { */
  /*   cvReleaseVideoWriter(&vw); */
  /*   video_run = 0; */
  /* } */
  /* cvReleaseCapture(&capture); */
  Camera_End(capture, vw, video_run);
  cvDestroyWindow(windowName);
  return 0;
}
