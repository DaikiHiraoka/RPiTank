/*
  コンパイル用コマンド
  g++ -o send send.cpp -I/usr/local/include/opencv -lpthread -ldl -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -lm -lsocket -lnsl
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "camera.hpp"

using namespace std;
using namespace cv;

#define DISPLAY
#define FPS 5

/* fps合わせ */
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

int Camera_main(Command_Info *command, Thread_Arg *thread_arg) {
// int main(int argc, char *argv[]){
  //ソケットの設定
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);  //ポート番号
  // addr.sin_addr.s_addr = inet_addr("192.168.38.133"); //送信先IPアドレス
  // addr.sin_addr.s_addr = inet_addr("10.1.10.212"); //送信先IPアドレス
  addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //送信先IPアドレス

  //カメラの設定
  VideoCapture cap(0);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  if(!cap.isOpened()){
    cout << "Camera not Found!" << endl;
    return -1;
  }

  int key = 0;
  int end_flag = 0;
  bool video_run = false;
  bool streaimg;
  double w = 640, h = 360;
  Mat frame;
  Mat jpgimg;
  // cv::VideoWriter vw;
  cv::VideoWriter vw("test.avi", CV_FOURCC_DEFAULT, 10, Size(640, 480), true);  char file_name[128];
  time_t t;
  static const int sendSize = 65500;
  char buff[sendSize];
  vector<unsigned char> ibuff;
  vector<int> param = vector<int>(2);
  param[0] = CV_IMWRITE_JPEG_QUALITY;
  param[1] = 85;
  cv::Point point(2,28);
#ifdef DISPLAY
  cv::namedWindow("Send", CV_WINDOW_AUTOSIZE);
#endif

  /* calc fps*/
  int cnt, oldcnt;
  int startTime, endTime, diffTime;
  int fps;
  const double f = (1000 /cv::getTickFrequency());
  /* while(cvWaitKey(1) == -1){ */
  startTime = cv::getTickCount();
  while(1) {
    cap >> frame;

    /* send udp streaming */
    if(streaimg) {
      imencode(".jpg", frame, ibuff, param);
      if(ibuff.size() < sendSize ){
	for(int i=0; i<ibuff.size(); i++) buff[i] = ibuff[i];
	sendto(sock, buff, sendSize, 0, (struct sockaddr *)&addr, sizeof(addr));
	jpgimg = imdecode(Mat(ibuff), CV_LOAD_IMAGE_COLOR);
      }
      ibuff.clear();
    }

    /* video output */
    if(video_run) {
      if(vw.isOpened()){
	vw << frame;
	cout << " video write " << endl;
      } else {
	cout << "Video Writer ERROR !!!" << endl;
	vw.release();
	video_run = false;
      }
    }

    /* calc fps  */
    endTime = cv::getTickCount();
    diffTime = (int)((startTime - endTime) * f);
    if (diffTime >= 1000) {
      startTime = endTime;
      fps = cnt - oldcnt;
      oldcnt = cnt;
    }

#ifdef DISPLAY
    std::ostringstream os;
    os << fps;
    std::string number = os.str();
    cv::putText(frame, number, point, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0,0,200), 2, CV_AA);
    cv::imshow("Send", frame);

    key = cv::waitKey(1);
    if(key > 1000000) {
      key -= 0x100000;
    }
    cout << "key: " << key << endl;

#endif
    switch(key) {
    case 113: /* q */
      end_flag = 1;
      break;

    case 112: /* p */
      t = time(NULL);
      strftime(file_name, sizeof(file_name), "./data/jpg/%Y_%m_%d_%H_%M_%S.jpg", localtime(&t));
      imwrite(file_name, frame);
      printf("%s\n", file_name);
      break;

    case 118: /* v */
      if(video_run == false) {
	t = time(NULL);
	strftime(file_name, sizeof(file_name), "./data/avi/%Y_%m_%d_%H_%M_%S.avi", localtime(&t));
	/* ビデオライター設定 */
	// vw = cvCreateVideoWriter(file_name/* "./data/avi/test.avi" */, CV_FOURCC ('M', 'J', 'P', 'G'), 5, cvSize ((int) w, (int) h), 1);
	/* vw.open(file_name, CV_FOURCC_DEFAULT, 5, cv::Size ((int) w, (int) h), true); */
	video_run = true;
	printf("Video START: %s\n", file_name);
      } else {
	/* cvReleaseVideoWriter(&vw); */
	printf("Video STOP: %s\n", file_name);
	vw.release();
	video_run = false;
      }
      break;

    default:
      break;
    }

    key = 0;

    cnt++;
    if(end_flag) break;
  } /* loop end */
  close(sock);

  cap.release();
  if(video_run) {
    vw.release();
    video_run = false;
  }
  return 0;
}
