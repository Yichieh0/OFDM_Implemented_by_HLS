# OFDM_Implemented_by_HLS

### > Motivation：

Orthogonal Frequency Division Multiplexing (OFDM) is a multiplexing technology which is widely used in 4G and 5G. The system utilize OFDM’s orthogonal property, we can set subcarriers closer in the frequency domain. Compared with single carrier system, we can transmit more data into same bandwidth. In this way, we can reach higher transmit rate. And due to its narrowband property, we can use a one-tap equalizer for channel compensation, which can reduce the complexity a lot. In this project, we tried to construct an end-to-end OFDM communication system by HLS.

![image](https://user-images.githubusercontent.com/102524142/174599266-031b3a6c-852f-442a-858c-c50b011f02cd.png)

### > Problem Statement：

In wireless communication system, multipath and fading noise are the two main purposes we would like to solve. We can’t control how signal transmitted from transmitter to receiver in wireless system. Each path may arrived receiver through different channel fading gain and wastes different time. This may cause some difficulties for receiver to recover the transmit data. Due to the reason we mentioned, we tried to construct a system based on OFDM which have the ability to conquer all of the non-ideal effects.

![image](https://user-images.githubusercontent.com/102524142/174599731-0d55d8b7-5141-48d4-a452-58ff461b25d4.png)

### > Project Scope
 #### 。System Architecture：
 The block diagram below is the architecture we used in this project : 
 
 ![image](https://user-images.githubusercontent.com/102524142/174599880-a740197d-dfab-4da7-938d-f461f42bd2c8.png)

 #### 。Specification：
1. FFT Size : 64 with SSR = 2
2. CP Length : FFT Size/4
3. Pilot Location: Block Type, 8*i for i = 0,1,...
4. Modulation : 16-QAM
5. SNR : [10:5:30]
6. Channel Model : Multipath (3-tap,6-tap) with Gaussian random
7. Total Data Size : (32x32)x7=7168 pixels
8. Platform : U50

(The whole system can be easily extended to other design through modified the parameters in myfix4_dataflow_final/src/data_path.hpp)

### > Simulation Results：
#### 。SYNTH Results

1. Utilization :

The tables below is the hardware resources utilization comparison between the system before and after optimization : 

 - With II Violation : 
 
 ![image](https://user-images.githubusercontent.com/102524142/174608540-4d911c0b-5cb7-4415-b7dd-4564d120ef44.png)

 - Without II Violation : 

![image](https://user-images.githubusercontent.com/102524142/174608627-a63e5727-d7a8-48aa-adcf-15670a987898.png)

2. Latency : 

下兩張表為CSYNTH後latency的比較，由於Tx和Rx的整體內部都是dataflow的形式，因此在CSYNTH階段無法看出Tx和Rx的latency。

 - With II Violation : 
 
![image](https://user-images.githubusercontent.com/102524142/174608763-5f989d38-2ec6-4e1a-958b-72aa78b94f56.png)

 - Without II Violation : 

![image](https://user-images.githubusercontent.com/102524142/174608820-2d6e1cb0-1800-4a0a-8641-c4972ea0654c.png)

#### 。COSIM Results

下兩張表為COSIM後latency的比較，可以明顯地觀察到Tx和Rx端的latency下降非常多，代表dataflow有發揮它的功能。

- With II Violation :

![image](https://user-images.githubusercontent.com/102524142/174608918-490d3ab5-e814-44f5-a6a2-5a44381e1664.png)

- Without II Violation : 

![image](https://user-images.githubusercontent.com/102524142/174608967-db85d0c6-71b3-4452-bb79-3b236b95f35e.png)


#### 。HW Emulation Results

  -  With II Violation :
  
![image](https://user-images.githubusercontent.com/102524142/174609070-3cb7f18c-ece9-4ff1-8ab4-68e655776c3b.png)![image](https://user-images.githubusercontent.com/102524142/174609076-76de0177-0945-4d1d-b989-618d1fe8c6dd.png)

- Without II Violation:

![image](https://user-images.githubusercontent.com/102524142/174609143-17006aba-022c-473c-af85-96b17e48beca.png)![image](https://user-images.githubusercontent.com/102524142/174609153-b5c00d34-04d5-4cb4-816e-afa95de9e7b9.png)

input data size = 1064(~1024，接近一張32x32照片的大小)

![image](https://user-images.githubusercontent.com/102524142/174609290-e9df5780-bae5-4c08-8c37-853d17d56c08.png)

經過優化後，Without II Violation的Emulation time跑得比With II Violation快。


































