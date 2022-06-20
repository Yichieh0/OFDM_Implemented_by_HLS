# OFDM_Implemented_by_HLS

### > Motivation：

正交分頻多工(Orthogonal Frequency-Division Multiplexing , OFDM)被廣泛地運用在通訊系統中，利用將子載波互相正交使得不同carrier的訊號在頻寬上可以交疊而不互相干擾，相較於只使用single-carrier可以大量節省頻寬，藉此來提高throughput，並由於其narrowband的特性，可以使用較為簡單的one-tap equalizer來進行通道補償，有效的降低系統計算複雜度。

![image](https://user-images.githubusercontent.com/102524142/174599266-031b3a6c-852f-442a-858c-c50b011f02cd.png)

### > Problem Statement：

在無線通訊系統中，訊號傳送出去可能會經由不同的路徑到達接收端，每一條路徑的長度以及衰退程度都不同，可能導致在同一時間點接收端會接收到不同傳送訊號經過不同程度衰退的總和，如何在這樣的情況下將傳送訊號還原回來便是我們所要解決的問題。

![image](https://user-images.githubusercontent.com/102524142/174599731-0d55d8b7-5141-48d4-a452-58ff461b25d4.png)

### > Project Scope
 #### 。System Architecture：
 
 以下是我們這次的系統架構，這是一個end to end的OFDM無線通訊系統，在通道的部分除了多路徑之外會加入白高斯雜訊，我們會丟入多張32x32的灰階圖來進行測試，最後在接收端進行還原。
 
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

 #### 。 Identify the area of work
 1. Modulation/Demodulation
 在此次的專題中我們使用16QAM作為我們的調變方法，下圖分別為經過Normalization (a)前(b)後的16QAM星座圖，隨著QAM數的提升，雖然一次傳送的bit數增加，可以使大幅增加傳輸速率，但由於不論使用何種調變技術傳送能量都會被固定，經過Normalize後會導致在QAＭ數增加的同時，兩個點中間的距離變小，導致判定變得更嚴苛，若再經過通道以及雜訊的影響會造成在接收端解調時錯誤率上升。
 
 ![image](https://user-images.githubusercontent.com/102524142/174600195-46428ee5-d896-4e13-9ba1-6518100faf53.png)![image](https://user-images.githubusercontent.com/102524142/174600204-83d7f800-331b-475c-aa83-ede5f8afafed.png)
 
------------------------(a)Before Normalization-------------------------------(b)After Normalization----------------------------------------------

 2. IFFT/FFT
 在這裡我們使用Xilinx提供的DSP Library中的FFT，透過參數設定可以讓Library依照自己的設計運行，下圖為我們所設定的參數：
 
 ![image](https://user-images.githubusercontent.com/102524142/174601045-e3614021-c428-4bfd-a37c-ec00182cdbb8.png)

FFT Library可以透過transform_direction來控制使用的是FFT或是IFFT
其中由於我們是使用fixed point，因此使用SR_FFT_GROW_TO_MAX_WIDTH作為Scaling Mode，這種方法是在FFT內部計算的數值尚未達到最大值27個bit(DSP內部運算最大bit數)之前都不會進行truncation的行為，一直到計算結果超過27個bit之後才會進行小數位元的捨棄。









 
 
