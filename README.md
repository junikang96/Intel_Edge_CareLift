# 🛗 CareLift: 다중 디바이스 기반 스마트 엘리베이터 시스템

## 📌 개요

CareLift는 교통 약자(휠체어 사용자, 노약자 등)의 편리한 이동을 돕기 위해  
**음성 제어**, **RFID 인증**, **휠체어 감지**, **모터 제어** 등의 기능을 통합한  
**임베디드 기반 스마트 엘리베이터 시스템**입니다.

각 기능은 STM32, Raspberry Pi, Arduino Nano, Jetson Nano 등 디바이스를 기반으로 나뉘며,  
**TCP 기반의 서버-클라이언트 구조**를 통해 유기적으로 연동됩니다.

---

## 🧠 시스템 구성

| 디바이스 | 주요 기능 | 설명 |
|----------|-----------|------|
| **Raspberry Pi** | 음성 제어 클라이언트 (📌 담당 구현) | Google STT/TTS API 기반의 명령 인식 및 음성 안내, 서버와 TCP 통신 수행 |
| **STM32 Nucleo-64** | 엘리베이터 모터 제어 (공동 제작) | 스텝모터(PWM)로 층간 이동, 서보모터로 도어 제어, 명령 수신 시 동작 |
| **Arduino Nano** | 사용자 인증 | RFID 카드 리더를 통해 사용자를 인식하고, 인증 정보 서버로 전송 |
| **Jetson Nano** | 휠체어 감지 | YOLOv8 기반 객체 인식으로 휠체어 사용자를 탐지하여 서버로 전송 |
| **서버 (Raspberry Pi)** | 제어 허브 | 각 디바이스로부터 명령 수신 및 전달, 시스템 상태 관리 및 응답 수행 |

---

## 🧑‍💻 담당 역할

이 프로젝트는 팀 단위로 진행되었으며,  
저는 **음성 제어 클라이언트 시스템 구현**과 **엘리베이터 하드웨어 제작(공동 참여)**를 담당하였습니다.

### 🎤 음성 인식 클라이언트 (Raspberry Pi)

- ALSA를 활용한 마이크 입력 캡처 및 음량 트리거 조건 구현  
- Google STT API로 "엘리베이터 시작" 명령어 인식  
- Google TTS API를 통해 mp3 음성 안내를 재생  
- 서버와 TCP 소켓 통신을 통해 명령어 전송 및 응답 수신  
- 오디오 캡처, 서버 수신, STT 요청, TTS 재생을 **멀티스레드 구조로 병렬 처리**

### 🔐 API Key 설정 방법

STT의 define.h와 TTS.c 파일에서 아래 부분을 본인의 Google API 키로 교체해야 정상 동작합니다.

```c
#define GOOGLE_STT_KEY "YOUR_API_KEY"
#define GOOGLE_TTS_KEY "YOUR_API_KEY"
```

### ⚙️ 하드웨어 제작 (공동)

- STM32 기반의 스텝모터 제어 및 서보모터 도어 시스템 조립 및 배선 참여  
- 센서 연결 및 회로 테스트, 하드웨어 동작 흐름에 대한 팀 설계 기여

---

## ⚙️ 사용 기술

| 분류 | 기술 |
|------|------|
| 음성 인식 | Google STT API, ALSA, C (Raspberry Pi) |
| 음성 피드백 | Google TTS API, mpg123 |
| 통신 방식 | TCP 소켓 통신 (서버 ↔ 클라이언트) |
| 구조 | 다중 스레드 (pthread) |
| 하드웨어 | STM32 Nucleo-64, Arduino Nano, ESP8266 |
| 인식 기술 | YOLOv8 (Jetson Nano), RFID 리더기 |
| 개발 도구 | STM32CubeIDE, PlatformIO, Jupyter Notebook |

---

## 🎥 시연 영상

- [👉 버튼 제어 시연 영상](https://youtube.com/shorts/Synbe0SKUcM?feature=share)
- [👉 음성 인식 제어 시연 영상](https://youtube.com/shorts/OxAtrQXdJtA?feature=share)
- [👉 휠체어 우선탑승 시연 영상](https://youtube.com/shorts/Th-BcP5JXi0?feature=share)

---

## 📁 폴더 구조

```bash
Intel_Edge_Final_Project_CareLift/
├── arduino/                      # RFID 인증 및 게이트 제어어 (Arduino Nano)
│   └── edge_elevator_done.ino
│
├── client_jetson_outside/        # 휠체어 감지 클라이언트 (Jetson Nano)
│   ├── backup_models/            
│   ├── srcs/                     
│   ├── best.pt                   
│   ├── requirements.sh
│   └── Makefile
│
├── client_raspberrypi/           # 음성 인식 클라이언트 (Raspberry Pi)
│   ├── STT/
│   │   ├── incs/                 
│   │   ├── srcs/                 
│   │   ├── voices/               
│   │   └── Makefile
│   └── TTS/                      # TTS 변환 및 음성 파일 생성성
│
├── server/                       # 제어 서버
│   ├── incs/
│   ├── srcs/
│   ├── server                   
│   └── Makefile
│
├── stm32/                        # 엘리베이터 하드웨어 제어 (STM32CubeIDE)
│   ├── Core/
│   ├── Drivers/
│   └── fin_pro.ioc
│
├── wheelchair_yolo/             # Jetson용 YOLO 휠체어 감지 Jupyter 코드
│   └── wheelchair.ipynb
│
├── images/             
│   ├── CareLift System – Front View
│   └── CareLift System – Rear View
│ 
├── .gitignore
├── LICENSE
└── README.md
