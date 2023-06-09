# Smith-Waterman Algorithm Implementation
This project provides a C implementation of the Smith-Waterman algorithm, which is a popular algorithm used for sequence alignment. The implementation offers a serial version of the algorithm, as well as parallel implementations that use OpenMP threads to speed up the execution. The parallelization is performed at both a coarse level and a fine level to optimize performance on different hardware.

# Features
* Provides a fast and accurate implementation of the Smith-Waterman algorithm for sequence alignment
* Includes serial and parallel implementations using OpenMP threads to optimize performance
* Provides both coarse and fine-level parallelization to optimize performance on different hardware
* Includes a comprehensive set of test cases and a sample dataset to help evaluate the accuracy and performance of the algorithm

# Instructions
Run all the tests by executing the run.sh script.

To download the large datasets used in the Smith Waterman algorithm implementation, please follow this [Google Drive link](https://drive.google.com/drive/folders/1x3cBA8keJrkn8WwQuldyE3MHq_-H3mnn?usp=share_link). These datasets will allow you to run the algorithm with both fine and coarse threads, and observe larger differences in running time. Please modify the run.sh file accordingly, by removing the commented lines. 
