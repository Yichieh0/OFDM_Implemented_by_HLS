# OFDM_Implemented_by_HLS

### > Motivation：

正交分頻多工(Orthogonal Frequency-Division Multiplexing , OFDM) 

被廣泛地運用在通訊系統中，利用將子載波互相正交使得不同 carrier 的訊號在頻寬上可以交疊而不互相干擾，
相較於只使用 single-carrier 可以大量節省頻寬，藉此來提高throughput，並由於其 narrowband 的特性，
可以使用較為簡單的 one-tap equalizer 來進行通道補償，有效的降低系統計算複雜度。

![image](https://user-images.githubusercontent.com/102524142/174599266-031b3a6c-852f-442a-858c-c50b011f02cd.png)

### > Problem Statement：

In wireless communication system, multipath and fading noise are the two main purposes we would like to solve. We can’t control how signal transmitted from transmitter to receiver in wireless system. Each path may arrived receiver through different channel fading gain and wastes different time. This may cause some difficulties for receiver to recover the transmit data. Due to the reason we mentioned, we tried to construct a system based on OFDM which have the ability to conquer all the non-ideal effects

![image](https://user-images.githubusercontent.com/102524142/174599731-0d55d8b7-5141-48d4-a452-58ff461b25d4.png)

### > Project Scope
 #### 。System Architecture：
 
 以下是我們這次的系統架構，這是一個end to end的OFDM無線通訊系統，在通道的部分除了多路徑之外會加入白高斯雜訊，
 我們會丟入多張32x32的灰階圖來進行測試，最後在接收端進行還原。 
 
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

3. Pilot Insertion/Channel Equalizer

為了抵抗multipath所造成的不理想效應，會事先對傳送訊號進行處理，在這裡我們選擇使用Block type，時域上每個時間都會有pilot，對於通道在時域變化快的情況比較好，而頻域上固定每格8或4個子載波插入pilot(已知訊號)，使接收端能夠用放pilot的子載波做通道估測，如下圖：

![image](https://user-images.githubusercontent.com/102524142/174601271-aa1cf0c5-e0cf-4ede-b0d9-e0a882adfc94.png)

Block-type會在固定的carrier位置放上pilot，並在每個時間都會進行估測，先計算出該pilot位置上的通道增益，其餘的subcarrier使用內插法以及外插法來取得近似的fading值。
	
我們所使用的channel如下：

![image](https://user-images.githubusercontent.com/102524142/174601382-03cafb95-389b-40ad-a5ed-bd753b24f3d0.png)![image](https://user-images.githubusercontent.com/102524142/174601392-922caf52-bd67-4c1a-bace-43dd131aedcc.png)

--------------------------------(a) 3 taps-------------------------------------------------------(b) 6 taps--------------------------------------------------------

在通道估測的部分我們會使用兩種方法來進行通道增益近似的計算，分別是內插法(interpolation)以及外插法(extrapolation)

![image](https://user-images.githubusercontent.com/102524142/174606139-ea734dd5-9caa-439a-a45e-5dc5087adc3d.png)

a) 內插法：

使用最相鄰的左右兩個pilot根據比例來近似當下的通道增益。

b) 外插法：

	由於最後七個data會沒有右邊的pilot來進行內插，因此在這裡我們選擇使用外插法進行通道增益的估測。
 
並且由於OFDM的narrowband特性，我們可以使用one-tap equalizer，直接將FFT的結果除以估測出來的channel fading進行補償。

4. CP Insertion/CP Removal

當接收端抓訊號的起始位置不正確時，若是以起始位置左偏，由於CP的特性使左偏抓到的訊號與原訊號有circular shift，相對於頻域會多乘exponential做相位旋轉，此項會被通道估測及補償消去掉，使左偏的訊號能成功解回來。而右偏的訊號會因為抓到下一個symbol的訊號，因此無法順利將原訊號解回來。

5. Multipath Channel

在Multipath Channel中我們使用了Xilinx的Random Library(MT19937IcnRng)，除了當作AWGN，也會為通道加上隨機性，通道的表示式如下所示：

h[t]=h_gain[t]*n[t]

上述的h_gain為通道在該時間點通道的gain，n用來賦予通道隨機性，最後h代表當下的通道增益。

在這裡我們使用的通道增益大小分別為3-tap的[0.5, 0.31, 0.19]以及6-tap的[0.7943, 0.3981, 0.3162, 0.2512, 0.1778, 0.1259]，通道的能量加總會等於1，使訊號經過channel後不會改變總能量為1。

 #### 。 Host Program

 ##### System structure:

 ![image](https://user-images.githubusercontent.com/102524142/174606962-567528e0-7f61-4c16-bd9a-1a7637520940.png)

將主kernal前後用AXI Stream與mem_read、mem_write連接，mem_read是將讀到的值傳給top_module，mem_write是將top_module出來的值write出來，算完的結果再回傳到host做比較，計算Bit error rate、symbol error rate及pixel error rate。

### > Optimization - II Violation

在初步確認CSIM結果正確之後進入Synthesis的部分，在Synthesis Report中可以看到FPGA資源的Ultilization以及Throughput等相關資訊，在這裡我們要對系統的II進行優化。

由於HLS tool在編譯的過程中可能會自動加入一些pragma來對系統進行優化，因此我們傾向於更改程式撰寫方式來化解II Violation的問題。

在優化前我們已經在主要的3個外部Kernel

 #### 。Tx需優化問題: 
 
 ![image](https://user-images.githubusercontent.com/102524142/174607212-9070ef55-4424-4b72-98d9-262e117971fa.png)

下圖為在優化前的II Violation狀況，可以看到在Tx端我們在一開始的第63行的for迴圈(對應到pixl_to_symbol以及QAM)、pilot insertion以及第98行的for迴圈(對應到Cp_insertion)有II violation的狀況。

![image](https://user-images.githubusercontent.com/102524142/174607264-07387a5d-2bf9-49a4-95fd-9e03d1ecbbe6.png)

1. pilot_insertion

![image](https://user-images.githubusercontent.com/102524142/174607305-c0b549cf-d6ae-4fa7-8ae1-6a11c101e6ab.png)

![image](https://user-images.githubusercontent.com/102524142/174607320-b06ee491-788d-4cae-8c77-32226613db2f.png)

 在處理pilot_insertion的問題時我們嘗試了兩種方法，一種是將整個pilot insertion的行為提出到Tx，提出後的結果如下圖所示：

 ![image](https://user-images.githubusercontent.com/102524142/174607361-a783c5e2-61a3-4abc-b7bb-d4cc91c3aa8e.png)

 根據完成後的結果可以看出，pilot_insertion所造成的II Violation的確被消除，但我們認為不同Block之間應該要分開表示，因此我們嘗試在pilot_insertion的內部也加入dataflow，並且將for迴圈移入kernel內部，藉此來消除直接使用for迴圈呼叫pilot_insertion module所造成的memory dependency。

 2. Cp_insertion

 接下來處理Cp_insertion造成的II Violation，如下圖所示，可以觀察到這裡也是要移除使用for迴圈直接呼叫module所造成的memory dependency。

 ![image](https://user-images.githubusercontent.com/102524142/174607444-4b90efd8-f421-429d-b726-44d8fc2d1466.png)

 和pilot_insertion相同，Cp_insertion內部加dataflow，並將for迴圈移入kernel內部進行運算。

 3. pixl_to_symbol

 接下來是對pixl_to_symbol進行II Violation的處理，由於一個pixel有8個位元，但一個symbol只有4個位元，因此讀入一個pixel會讀出兩個symbol，
若是在同一個cycle內將兩筆資料write到同一個變數會造成memory dependency的問題，如下圖所示：

 ![image](https://user-images.githubusercontent.com/102524142/174607538-558a34d2-fc54-42d6-a318-57a8bdd9f0f7.png)

 因此我們將兩個output拆成兩個cycle寫進stream中(由圖(a)轉換成圖(b))，經過這項變動可以將原本一個cycle要write兩筆資料的變數拆開為一個cycle寫入一筆資料，這樣可以避免同時對一個變數做兩次寫入時所造成的memory dependency。

 ![image](https://user-images.githubusercontent.com/102524142/174607612-efa7cd9e-8dee-4809-ae1f-e2ede133bd50.png)

----------------------------------------------(a)---------------------------------------------

![image](https://user-images.githubusercontent.com/102524142/174607964-344504e3-6e24-427c-bc5b-da9a83e66c10.png)

----------------------------------------------(b)---------------------------------------------

下圖為將Tx端所有kernel都成功解決II Violation後的結果，從右側Pipelined的結果可以看到所有Tx內的kernel都成功使用dataflow，但由於我們使用了xilinx的dsp library，需要配合library需要的資料格式，我們需要將原本單一的stream資料讀成兩個一組的array stream，所以在Tx端需要使用for迴圈進行資料讀取，但也可以觀察到直接寫在Tx內部的for迴圈有使用pipeline。

![image](https://user-images.githubusercontent.com/102524142/174608113-93052ef0-74d4-4e4c-b71c-c7f02d6877b4.png)

### 。Rx待優化問題: 

![image](https://user-images.githubusercontent.com/102524142/174608176-48ac65f9-22d5-4a89-a804-064ee3e12fb7.png)

在Rx端的II Violation相較於Tx端較單純，如下圖所示：

![image](https://user-images.githubusercontent.com/102524142/174608240-89847ab7-1ec9-4876-8968-5d0a829a57c3.png)

可以看到II Violation出現在Rx中第44行的for迴圈(對應到讀取equalizer output的部分)。

根據下圖可以看出是因為有memory dependency造成的II Violation，因為在同一個cycle內會同時讀入Equal_out[0]及Equal_out[1]作為data_temp的實部以及虛部，但Equal_out是一個array，在沒有加入任何pragma的情況之下，array只會有單一接口可以進行讀取，因此為了讓array內的內容可以同時被讀取，需要加入array_partition的pragma使array可以同時讀取整個array所有位置的內容。

![image](https://user-images.githubusercontent.com/102524142/174608307-864689b1-ba65-4fde-b86a-01f01af8277d.png)

![image](https://user-images.githubusercontent.com/102524142/174608346-655a64db-b883-4fb5-a491-34df9f2a5eaa.png)

最後我們成功消除所有的II violation，以下為我們優化後最終的結果。

![image](https://user-images.githubusercontent.com/102524142/174608391-19470489-942b-4597-ba67-6d63734a629f.png)

### 。SYNTH Results

1. Utilization :

下兩張表分別為優化前後的硬體資源使用。

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

### 。COSIM Results

下兩張表為COSIM後latency的比較，可以明顯地觀察到Tx和Rx端的latency下降非常多，代表dataflow有發揮它的功能。

- With II Violation :

![image](https://user-images.githubusercontent.com/102524142/174608918-490d3ab5-e814-44f5-a6a2-5a44381e1664.png)

- Without II Violation : 

![image](https://user-images.githubusercontent.com/102524142/174608967-db85d0c6-71b3-4452-bb79-3b236b95f35e.png)


### 。HW Emulation Results

  -  With II Violation :
  
![image](https://user-images.githubusercontent.com/102524142/174609070-3cb7f18c-ece9-4ff1-8ab4-68e655776c3b.png)![image](https://user-images.githubusercontent.com/102524142/174609076-76de0177-0945-4d1d-b989-618d1fe8c6dd.png)

- Without II Violation:

![image](https://user-images.githubusercontent.com/102524142/174609143-17006aba-022c-473c-af85-96b17e48beca.png)![image](https://user-images.githubusercontent.com/102524142/174609153-b5c00d34-04d5-4cb4-816e-afa95de9e7b9.png)

input data size = 1064(~1024，接近一張32x32照片的大小)

![image](https://user-images.githubusercontent.com/102524142/174609290-e9df5780-bae5-4c08-8c37-853d17d56c08.png)

經過優化後，Without II Violation的Emulation time跑得比With II Violation快。


































