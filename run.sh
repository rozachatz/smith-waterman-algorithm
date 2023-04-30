echo "Compiling..."
if [ ! -d "src" ]; then
	echo "Error, no source files found."
else
	mkdir -p Build
	gcc src/serial_program_diagonals.c -o Build/serial
	gcc src/parallel_fine_openmp.c -fopenmp -o Build/openmp_fine
	gcc src/parallel_coarse_openmp.c -fopenmp -o Build/openmp_coarse
	gcc src/parallel_fine_pthreads.c -pthread -o Build/pthreads_fine
	gcc src/parallel_coarse_pthreads.c -pthread -o Build/pthreads_coarse
fi
sleep(10)
echo "Running the executables..."
if [ ! -d "Datasets" ]; then
	echo "Error.Insert a folder named Datasets with the appropiate txt files in it\n"
else
	 echo "Serial program running...(from D1 to D10)\n"
	./Build/serial -name "1" -input "Datasets/D1.txt" -gap -1 -match 3 -mismatch -1
	./Build/serial -name "2" -input "Datasets/D2.txt" -gap -1 -match 3 -mismatch -1
	./Build/serial -name "3" -input "Datasets/D3.txt" -gap -1 -match 3 -mismatch -1
	./Build/serial -name "4" -input "Datasets/D4.txt" -gap -1 -match 3 -mismatch -1
	./Build/serial -name "5" -input "Datasets/D5.txt" -gap -1 -match 3 -mismatch -1
	 echo "Parallel program for OMP FINE 2 THREADS running...(from D1 to D5)\n"
	./Build/openmp_fine -name "fine1_OMP_2" -input "Datasets/D1.txt" -gap -1 -match 3 -mismatch -1 -threads 2
	./Build/openmp_fine -name "fine2_OMP_2" -input "Datasets/D2.txt" -gap -1 -match 3 -mismatch -1 -threads 2
	./Build/openmp_fine -name "fine3_OMP_2" -input "Datasets/D3.txt" -gap -1 -match 3 -mismatch -1 -threads 2
	./Build/openmp_fine -name "fine4_OMP_2" -input "Datasets/D4.txt" -gap -1 -match 3 -mismatch -1 -threads 2
	./Build/openmp_fine -name "fine5_OMP_2" -input "Datasets/D5.txt" -gap -1 -match 3 -mismatch -1 -threads 2

	 echo "Parallel program for OMP FINE 4 THREADS running...(from D1 to D5)\n"
	./Build/openmp_fine -name "fine1_OMP_4" -input "Datasets/D1.txt" -gap -1 -match 3 -mismatch -1 -threads 4
	./Build/openmp_fine -name "fine2_OMP_4" -input "Datasets/D2.txt" -gap -1 -match 3 -mismatch -1 -threads 4
	./Build/openmp_fine -name "fine3_OMP_4" -input "Datasets/D3.txt" -gap -1 -match 3 -mismatch -1 -threads 4
	./Build/openmp_fine -name "fine4_OMP_4" -input "Datasets/D4.txt" -gap -1 -match 3 -mismatch -1 -threads 4
	./Build/openmp_fine -name "fine5_OMP_4" -input "Datasets/D5.txt" -gap -1 -match 3 -mismatch -1 -threads 4

	 echo "Parallel program for PTHREADS FINE 2 THREADS running...(from D1 to D5)\n"
	./Build/pthreads_fine -name "fine1_PTH_2" -input "Datasets/D1.txt" -gap -1 -match 3 -mismatch -1 -threads 2
	./Build/pthreads_fine -name "fine2_PTH_2" -input "Datasets/D2.txt" -gap -1 -match 3 -mismatch -1 -threads 2
	./Build/pthreads_fine -name "fine3_PTH_2" -input "Datasets/D3.txt" -gap -1 -match 3 -mismatch -1 -threads 2
	./Build/pthreads_fine -name "fine4_PTH_2" -input "Datasets/D4.txt" -gap -1 -match 3 -mismatch -1 -threads 2
	./Build/pthreads_fine -name "fine5_PTH_2" -input "Datasets/D5.txt" -gap -1 -match 3 -mismatch -1 -threads 2
	
	 echo "Parallel program for PTHREADS FINE 4 THREADS running...(from D1 to D5)\n"
	./Build/pthreads_fine -name "fine1_PTH_4" -input "Datasets/D1.txt" -gap -1 -match 3 -mismatch -1 -threads 4
	./Build/pthreads_fine -name "fine2_PTH_4" -input "Datasets/D2.txt" -gap -1 -match 3 -mismatch -1 -threads 4
	./Build/pthreads_fine -name "fine3_PTH_4" -input "Datasets/D3.txt" -gap -1 -match 3 -mismatch -1 -threads 4
	./Build/pthreads_fine -name "fine4_PTH_4" -input "Datasets/D4.txt" -gap -1 -match 3 -mismatch -1 -threads 4
	./Build/pthreads_fine  -name "fine5_PTH_4" -input "Datasets/D5.txt" -gap -1 -match 3 -mismatch -1 -threads 4


fi

