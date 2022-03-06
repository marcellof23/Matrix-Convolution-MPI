# **Tugas Besar 1 IF3230**

## 1. **Jelaskan cara kerja program Anda, terutama pada paralelisasi yang Anda implementasikan berdasarkan skema di atas**

Pertama-tama program akan diinisialisasi dengan MPI_Init untuk memulai environment dari eksekusi MPI, lalu dilanjutkan dengan memasukkan ukuran dan peringkat pada group yang terasosiasi dengan komunikator dengan API MPI_Comm_size dan MPI_Comm_rank. Selanjutnya, kamik membuat tipe struktur dari matriks yang dibuat, yang terdiri dari matriks itu sendiri, jumlah baris dan jumlah kolom. Setelah itu kami membuat strutkur tipe data dengan menggunakan API MPI_Type_create_struct dan melakukan commit pada struktur data tersebut dengan API MPI_Type_commit.

Kemudia dilakukan pengecekan, apabila rank sekarang merupakan BROADCASTER_RANK (Yang dalam program kami ini kami definisikan dengan 0) maka kita akan memulai perhitungan waktu dan melakukan seluruh input-input pada matriks kernel maupun matriks target. Selanjutnya kita melakukan kalkulasi pada variable array send_counts dan displacements. send_counts disini berperan untuk menghitung jumlah data pada setiap proses dan displacement merupakan perpindahan offset dimana setiap segment data mulai.

Setelah itu program kami melakukan broadcast message dari proses dengan rank root atau pada program kami disebut dengan BROADCAST_RANK pada semua proses lain pada group tersebut termasuk dirinya sendiri . Broadcast dilakukan pada buffer kernel_matrix, send_counts dan displacements. Selanjutnya kami memasang MPI_Barrier yang bertujuan untuk menunggu setelah semua member dari group tersebut telah memasukki barier ini. Kemudia kami memecah buffer hingga beberapa part untuk semua task pada grop dengan API MPI_Scatterv. Setelah itu dapat dilakukan komputasi pada matrix konvolusi dan menghitung data rangenya.

Pada potongan program selanjutnya, dilakukan perhitungan matrix konvolusi dan data rangenya secara parallel. Pada komputasi konvolusi, kami menambahkan hashtag pragma omp parallel for untuk memunculkan sebuah group yang terdiri dari thread lalu membagi loop iterasi diantara thread yang muncul tersebut. Pada komputasi untuk mendapatkan data rangenya, kami menggunakan #pragma omp parallel reduction untuk max dan min. Parameter reduction ini berguna untuk melakukan reduksi untuk semua variabel skalar pada matriks kami dengan melakukan penyalinan secara privat pada setiap thread lalu nilai akhir pada salinan privat tersebut akan digabung dan diletakkan kembali pada original value pada variabel reduksi bersama. Selanjutnya hasil tersebut akan di-_sort_ dan akan digabungkan kembali data-data dari semua proses yang telah dibagi dengan API MPI_Gatherv. Terakhir dilakukan sort dan perhitungan median dan mean jika rank sekarang sama dengan BROADCASTER_RANK setelah itu baru eksekusi environment MPI diterminasi dengan MPI_Finalize.

---

## 2. **Dari waktu eksekusi terbaik program paralel Anda, bandingkan dengan waktu eksekusi program sekuensial yang diberikan. Analisis mengapa waktu eksekusi program Anda bisa lebih lambat / lebih cepat / sama saja.**

### Serial

| TC  |   Time Required    |
| :-: | :----------------: |
| TC1 |  0.052763 seconds  |
| TC2 |  15.735753 second  |
| TC3 | 12.245259 seconds  |
| TC4 | 141.187519 seconds |

### Paralel dengan Waktu Eksekusi Terbaik

#### 3 Core 5 Threads

| Test Case |   Time Required   |
| :-------: | :---------------: |
|    TC1    | 0.961302 seconds  |
|    TC2    | 5.381746 seconds  |
|    TC3    | 7.235308 seconds  |
|    TC4    | 81.494726 seconds |

### Analisis

Waktu eksekusi terbaik yang kami peroleh melalui implementasi parallel computing memberikan hasil yang lebih cepat daripada implementasi serial, hal ini karena adanya proses paralelisasi terhadap input matriks yang diberikan sehingga setiap pemrosesan konvolusi dan pencarian selisih nilai terbesar dan terkecil dala matriks hasil konvolusi dapat dilakukan secara serentak sehingga tidak terjadi "antrian" pemrosesan yang panjang akibat pemrosesan matriks yang serial, mengingat setiap proses yang di-assign untuk memproses matriks saling independen dan tidak memengaruhi jalannya proses lain.

Jika kita tinjau dengan mempertimbangkan jumlah node dan jumlah thread setiap node maka kita bisa membahasnya secara terpisah. Pertama kita tinjau dari jumlah node, hal-hal yang berhubungan dengan bertambahnya jumlah node adalah semakin banyaknya jumlah processor yang bisa dimanfaatkan secara parallel (dalam hal kelompok kami itu adalah jumlah node karena host kami memiliki jumlah processor default yaitu 1), kita memanfaatkannya dengan membagi data yang perlu diproses ke n buah node. Tetapi kita harus ingat bahwa node ini menyebabkan ada tambahan delay terutama pada network delay dan juga ketika melakukan scatter dan gather karena perlu menyimpan data yang dishare ke buffer (ini dihandle secara internal oleh OpenMPI).

Peninjauan kedua dilakukan terhadap jumlah thread yang digunakan dalam pemrosesan oleh OpenMP. Thread yang benar-benar bisa dijalankan secara parallel bukan jumlah thread OMP yang kita specify melainkan dibatasi oleh derajat paralelisasi mesin, misalnya jumlah CPU Core. Apabila thread lebih banyak daripada CPU core dan juga ditambah dengan fakta bahwa semua thread yang kita miliki sifatnya adalah CPU bound dan bukan I/O bound maka tidak ada gunanya apabila CPU mesin kita oversaturated yaitu ketika jumlah thread/process yang ingin dijalankan lebih besar dibanding derajat paralelisasi mesin. Kemudian ditambah lagi dengan fakta bahwa akan dilakukan banyak context switching yang sebenarnya tidak ada gunanya di kasus ini. Lalu kita tinjau seberapa efisien paralelisasi OpenMP yang kita gunakan. Untuk konvolusi matriks, semua operasi sifatnya adalah thread safe dan tidak ada mutex yang harus diberikan sehingga semuanya benar-benar bisa berjalan secara paralel namun apabila kita melihat bagian kode untuk data range kita melihat ada klausa reduction untuk 2 variabel dan secara internal OpenMP akan membuat semua operasi ke variabel tersebut menjadi atomic sehingga hanya ada 1 thread yang bisa menjalankan bagian kode tersebut. Hal ini akan menjadi bottleneck karena dalam blok kode yang diparalelisasi didominasi oleh operasi yang atomic sehingga sebagian besar thread akan menunggu resource kedua variabel ini belum lagi ada overhead untuk membuat kode tersebut menjadi atomic.

Argumen terakhir yang akan kami jelaskan adalah mengenai proses yang CPU bound. Dengan mengasumsikan scheduler kernel linux yang digunakan adalah CFS (completely fair scheduler) maka process yang akan diprioritaskan adalah proses yang I/O bound. Hal ini ditambah dengan fakta bahwa banyak mahasiswa yang melakukan SSH ke mesin sehingga akan banyak process yang sifatnya I/O bound (karena menunggu stdin dari mahasiswa lain atau membaca buffer dari file) sehingga setiap kali mahasiswa menginput teks ke stdin maka process yang menghandle terminal yang di-ssh akan diprioritaskan oleh CPU dan dijalankan di CPU, membuat process kita yang CPU bound dimasukkan ke dalam antrian dengan state runnable yang menunggu. Ditambah lagi dengan fakta bahwa CPU yang ada di mesin memiliki core yang sedikit sehingga derajat paralelisasi kecil dan juga bisa saja banyak mahasiswa yang menjalankan program dalam waktu yang bersamaan sehingga kompleksitas scheduling bertambah (sekali lagi ini mengasumsikan scheduler yang digunakan adalah CFS dan bukan O(1) scheduler karena version kernel yang digunakan > 2.6) dan juga program harus melakukan time sharing dengan lebih banyak process (berbanding lurus dengan jumlah program yang dijalankan).

---

## 3. **Jelaskan secara singkat apakah ada perbedaan antara hasil program serial dan program paralel Anda**

Untuk hasil output program serial dan program paralel kami, tidak terdapat perbedaan sama sekali. Sehingga dapat disimpulkan bahwa algoritma perhitungan matriks konvolusi dan pencarian selisih nilai maksimum dan minimum dari elemen matriks dapat diimplementasikan secara paralel dan aman.

---

## 4. **Variasikan jumlah node OpenMPI yang berpartisipasi dan jumlah thread OpenMP yang digunakan. Gunakan percobaan-percobaan dengan parameter berikut**

## Serial

| TC  |   Time Required    |
| :-: | :----------------: |
| TC1 |  0.052763 seconds  |
| TC2 |  15.735753 second  |
| TC3 | 12.245259 seconds  |
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

| Test Case |   Time Required    |
| :-------: | :----------------: |
|    TC1    |  4.887166 seconds  |
|    TC2    | 13.892496 seconds  |
|    TC3    | 29.490713 seconds  |
|    TC4    | 235.384670 seconds |

### Analisis

Tampak dari waktu eksekusi yang ditampilkan diatas, terjadi pengurangan waktu eksekusi sampai pada percobaan dengan puncak eksekusi tercepat diperoleh dengan menggunakan 3 core dan 5 thread. Hal ini hampir sesuai dengan ekspektasi kami yaitu puncak eksekusi tercepat diperoleh dengan menggunakan 4 core dan 5 thread. Alasan mengapa kami mengekspektasikan bahwa puncak eksekusi tercepat diperoleh dengan menggunakan 4 core dan 5 thread adalah karena semakin banyak jumlah core dalam lingkungan yang ideal, waktu eksekusi akan semakin cepat hingga waktu eksekusi didominasi oleh network delay. Alasan mengapa menurut kami 4 core 5 thread lebih cepat daripada 4 core 16 thread adalah karena semakin banyak jumlah thread, maka semakin banyak pula context switching yang harus dilakukan oleh mesin apabile jumlah thread yang di-specify lebih besar dari derajat paralelisasi yang bisa dijalankan oleh mesin.

OpenMP memengaruhi waktu eksekusi program kami dengan menjalankan sebuah blok kode dalam program kami ke dalam beberapa thread, sehingga blok kode tersebut bisa berjalan secara paralel sesuai dengan jumlah thread yang diberikan.

OpenMPI memengaruhi waktu eksekusi program kami dengan membagi data yang dieksekusi program ke beberapa mesin yang independen untuk dieksekusi secara paralel.

Perlu diperhatikan bahwa pasti data yang dikumpulkan bisa tidak sesuai dengan teori karena poin terakhir yang saya sebutkan pada analisis di awal readme karena banyak sekali parameter lain yang bermain di hasil percobaan. Mungkin bisa dibantah dengan argumen bahwa CPU bisa di-limit untuk setiap mahasiswa yang melakukan ssh ke mesin tetapi perlu dicatat bahwa limit ini sifatnya logical pada OS dan bukan hardware.

---
