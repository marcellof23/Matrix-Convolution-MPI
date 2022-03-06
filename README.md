# **Tugas Besar 1 IF3230**
## 1. **Jelaskan cara kerja program Anda, terutama pada paralelisasi yang Anda implementasikan berdasarkan skema di atas**

Pertama-tama program akan diinisialisasi dengan MPI_Init untuk memulai environment dari eksekusi MPI, lalu dilanjutkan dengan memasukkan ukuran dan peringkat pada group yang terasosiasi  dengan komunikator dengan API MPI_Comm_size dan MPI_Comm_rank. Selanjutnya, kamik membuat tipe struktur dari matriks yang dibuat, yang terdiri dari matriks itu sendiri, jumlah baris dan jumlah kolom. Setelah itu kami membuat strutkur tipe data dengan menggunakan API MPI_Type_create_struct dan melakukan commit pada struktur data tersebut dengan API MPI_Type_commit.

Kemudia dilakukan pengecekan, apabila rank sekarang merupakan BROADCASTER_RANK (Yang dalam program kami ini kami definisikan dengan 0) maka kita akan memulai perhitungan waktu dan melakukan seluruh input-input pada matriks kernel maupun matriks target. Selanjutnya kita melakukan kalkulasi pada variable array send_counts dan displacements. send_counts disini berperan untuk menghitung jumlah data pada setiap proses dan displacement merupakan perpindahan offset dimana setiap segment data mulai. 

Setelah itu program kami melakukan broadcast message dari proses dengan rank root atau pada program kami disebut dengan BROADCAST_RANK pada semua proses lain pada group tersebut termasuk dirinya sendiri . Broadcast dilakukan pada buffer kernel_matrix, send_counts dan displacements. Selanjutnya kami memasang MPI_Barrier yang bertujuan untuk menunggu setelah semua member dari group tersebut telah memasukki barier ini. Kemudia kami memecah buffer hingga beberapa part untuk semua task pada grop dengan API MPI_Scatterv. Setelah itu dapat dilakukan komputasi pada matrix konvolusi dan menghitung data rangenya.

Pada potongan program selanjutnya, dilakukan perhitungan matrix konvolusi dan data rangenya secara parallel. Pada komputasi konvolusi, kami menambahkan hashtag pragma omp parallel for untuk memunculkan sebuah group yang terdiri dari thread lalu membagi loop iterasi diantara thread yang muncul tersebut. Pada komputasi untuk mendapatkan data rangenya, kami menggunakan   #pragma omp parallel reduction untuk max dan min. Parameter reduction ini berguna untuk melakukan reduksi untuk semua variabel skalar pada matriks kami dengan melakukan penyalinan secara privat pada setiap thread lalu nilai akhir pada salinan privat tersebut akan digabung dan diletakkan kembali pada original value pada variabel reduksi bersama. Selanjutnya hasil tersebut akan di-*sort* dan akan digabungkan kembali data-data dari semua proses yang telah dibagi dengan API MPI_Gatherv. Terakhir dilakukan sort dan perhitungan median dan mean jika rank sekarang sama dengan BROADCASTER_RANK setelah itu baru eksekusi environment MPI diterminasi dengan MPI_Finalize.   

------------------------------------------
## 2. **Dari waktu eksekusi terbaik program paralel Anda, bandingkan dengan waktu eksekusi program sekuensial yang diberikan. Analisis mengapa waktu eksekusi program Anda bisa lebih lambat / lebih cepat / sama saja.**

```
asdfasdf
```
------------------------------------------
## 3. **Jelaskan secara singkat apakah ada perbedaan antara hasil program serial dan program paralel Anda**

```
asdfasdf
```
------------------------------------------
## 4. **Variasikan jumlah node OpenMPI yang berpartisipasi dan jumlah thread OpenMP yang digunakan. Gunakan percobaan-percobaan dengan parameter berikut**


## Serial 
| TC | Time Required |
| :---: | :---: | 
| TC1 | 0.052763 seconds |
| TC2 | 15.735753 second |
| TC3 | 12.245259 seconds |
| TC4 | 141.187519 seconds |

## Paralel 

2 Core 5 Threads
| Test Case | Time Required |
| :---: | :---: | 
| TC1 | 1.634647 seconds |
| TC2 | 4.471616 seconds |
| TC3 | 11.722067 seconds|
| TC4 | 105.991456 seconds |

2 Core 16 Threads 
| Test Case | Time Required |
| :---: | :---: | 
| TC1 | 1.456144 seconds |
| TC2 | 6.971852 seconds |
| TC3 | 8.999443 seconds |
| TC4 | 101.482686 seconds |

3 Core 5 Threads
| Test Case | Time Required |
| :---: | :---: | 
| TC1 | 0.961302 seconds |
| TC2 | 5.381746 seconds |
| TC3 | 7.235308 seconds |
| TC4 | 81.494726 seconds |

3 Core 16 Threads
| Test Case | Time Required |
| :---: | :---: | 
| TC1 | 0.060532 seconds |
| TC2 | 7.445019 seconds |
| TC3 | 10.003477 seconds |
| TC4 | 104.910649 seconds |

4 Core 5 Threads
| Test Case | Time Required |
| :---: | :---: | 
| TC1 | 3.255436 seconds |
| TC2 | 18.435459 seconds |
| TC3 | 27.155610 seconds |
| TC4 | 207.701387 seconds |

4 Core 16 Threads

| Test Case | Time Required |
| :---: | :---: | 
| TC1 | 4.887166 seconds|
| TC2 | 13.892496 seconds |
| TC3 | 29.490713 seconds |
| TC4 | 235.384670 seconds |
------------------------------------------
