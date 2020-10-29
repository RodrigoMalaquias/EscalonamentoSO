// EscalonamentoSo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <iomanip>
#include <cstdlib>


using namespace std;
int n;

// Integrante:
//Rodrigo Malaquias Viana


void swap(int* xp, int* yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}
void comparatorValues(int vector[], int vector2[], int vector3[])
{
	int i = 0, j = 0;
	for (i = 0; i < n - 1; i++) {
		// Last i elements are already in place  
		for (j = 0; j < n - i - 1; j++) {
			if (vector[j] > vector[j + 1]) {
				swap(&vector[j], &vector[j + 1]);
				swap(&vector2[j], &vector2[j + 1]);
				swap(&vector3[j], &vector3[j + 1]);
			}
		}
	}
}
void fifo(int vector[], int vector2[], int vector3[]) {

	ofstream txtWriting;
	//Preparação para Escrita
	txtWriting.open("EscalonamentoSaida.txt", fstream::app);

	comparatorValues(vector, vector2, vector3);

	float waitingTime = 0;
	float durationTime = 0;
	float responseTime;
	int i = 0;
	//Calculo Tempo de Espera
	while(i != n)
	{
		if (i != 0)
		{
			waitingTime += durationTime - vector[i];
			durationTime += vector2[i];
		}
		else
		{
			waitingTime = 0;
			durationTime = vector[i] + vector2[i];
		}
		i++;
	}

	waitingTime = waitingTime / n;
	responseTime = waitingTime;
	
	txtWriting << fixed << setprecision(2);
	txtWriting << "FIFO " << waitingTime << " " << responseTime << endl;
}
void prio(int vector[], int vector2[], int vector3[]) {

	ofstream txtWriting;
	//Preparação para Escrita
	txtWriting.open("EscalonamentoSaida.txt", fstream::app);

	comparatorValues(vector, vector2, vector3); // prioridade/chegada/duracao

	float waitingTime = 0;
	float durationComplete = 0;
	float responseTime = 0;
	float somap = 0;
	int arrivalDifference = 0;
	for (int i = 0; i < n; i++)
	{
		durationComplete += vector3[i];
	}

	// Calculo Tempo de Espera
	for (int i = (n - 1), j = (n - 2); i >= 0; i--, j--)
	{
		durationComplete -= vector3[i];
		if ((vector2[j] - vector2[i]) < 0 and j != -1)
		{
			arrivalDifference += vector2[i] - vector2[j];
		}
		waitingTime += durationComplete;

	}

	waitingTime -= arrivalDifference;

	// Calculo Tempo de Resposta
	for (int i = 0, j = 1; i < n; i++, j++)
	{
		//Verificando todos os casos de prioridade
		if (vector[j] >= vector[i] and vector2[j] == vector2[i])
		{
			somap += vector3[i];
			responseTime += somap;
		}
		if (vector[j] == vector[i] and vector2[j] > vector2[i])
		{
			somap += (vector3[j] + vector2[j]) - vector2[i];
			responseTime += somap;
		}
		if (vector[j] > vector[i] and vector2[j] != vector2[i])
		{
			somap += 0;
		}

	}
	waitingTime /= n;
	responseTime /= n;

	txtWriting << fixed << setprecision(2);
	txtWriting << fixed << setprecision(2);
	txtWriting << "PRIO " << waitingTime << " " << responseTime << endl;
}
void rrq(int vector[], int vector2[], int vector3[]) {

	ofstream txtWriting;
	//Preparação para Escrita
	txtWriting.open("EscalonamentoSaida.txt", fstream::app);

	comparatorValues(vector, vector2, vector3);
	int total = 0, x, counter = 0, quantum;
	int wait_time = 0;
	int* temp = new int[n];
	float waitingTime = 0;
	int firstStart = 0, respDif = 0, sumResp = 0;
	x = n;
	int i;
	//rrq(vectStart, vectDuration, vectPriority);
	quantum = 5;
	for (i = 0; i < n; i++)
	{
		temp[i] = vector2[i];
	}

	for (total = 0, i = 0; x != 0;)
	{
		if (temp[i] <= 5 && temp[i] > 0)
		{
			total = total + temp[i];
			temp[i] = 0;
			counter = 1;
		}
		else if (temp[i] > 0)
		{
			temp[i] = temp[i] - 5;
			total = total + 5;
		}
		if (temp[i] == 0 && counter == 1)
		{
			x--;
			wait_time += total - vector[i] - vector2[i];
			counter = 0;
		}
		if (i == n - 1)
		{
			i = 0;
		}
		else if (vector[i + 1] <= total)
		{
			i++;
		}
		else
		{
			i = 0;
		}
	}


	firstStart = vector[0];
	for (int i = 0, j = 1; i < n; i++, j++)
	{
		if (j != n) {
			while (vector[j] >= firstStart) {
				firstStart += 5;
			}
			if (i > 0) {
				respDif = (firstStart + 5) - vector[j];
			}
			else {
				respDif = firstStart - vector[j];
			}
			sumResp += respDif;
		}
	}

	waitingTime = wait_time / n;
	txtWriting << fixed << setprecision(2);
	txtWriting << "RRQ5 " << waitingTime << " " << sumResp / n << endl;
}
void srt(int vector[], int vector2[], int vector3[]) {
	ofstream txtWriting;
	//Preparação para Escrita
	txtWriting.open("EscalonamentoSaida.txt", fstream::app);

	comparatorValues(vector, vector2, vector3);

	int* at = new int[n];
	int* bt = new int[n];
	int* rt = new int[n];

	int endTime, smallest;
	float remainingTime = 0, time, waitingTime = 0;
	float resp = 0, resp2 = 0, aux = 0;
	bool bo = true;
	for (int i = 0; i < n; i++)
	{
		at[i] = vector[i];
		bt[i] = vector2[i];
		rt[i] = bt[i];
	}
	
	rt[9] = 9999;
	for (time = 0; remainingTime != n; time++) {
		smallest = 9;
		for (int i = 0; i < n; i++) {
			if (vector[i] <= time && rt[i] < rt[smallest] && rt[i]>0) {
				smallest = i;
			}
		}
		rt[smallest]--;
		if (rt[smallest] == 0) {
			remainingTime++;
			endTime = time + 1;
			waitingTime += endTime - vector2[smallest] - vector[smallest];
			if (bo) {
				resp += waitingTime - aux;
				resp2 = waitingTime - aux;
				bo = false;
			}
			if (resp == waitingTime) {
				bo = true;
				aux = waitingTime;
			}
		}

	}
	

	bool complete = false;
	int firstValueSomap = vector[0];
	int cont = 0;
	int somap = vector[0];
	float mediaResponse = 0;
	
	for (int i = 0; i < n; i++)
	{
		somap += vector2[i];
	}

	for (int i = 0,j = 1; i < n;i++,j++)
	{
		while (complete == false)
		{
			firstValueSomap += 1;
			if (firstValueSomap == vector[j]) {
				if (vector2[i]- firstValueSomap > vector2[j])
				{
					cont++;
				}
			}
			if (firstValueSomap == somap)
				complete = true;	
		}
	}
	
	if (cont > 0)
		mediaResponse = resp2 / n;
	else
		mediaResponse = resp / n;


	txtWriting << fixed << setprecision(2);
	txtWriting << "SRT " << (waitingTime * 1.0 / n) << " " << mediaResponse << endl;
}

int main()
{
	ifstream txtRepository; //Variavel que irá tratar da leitura do Txt.
	string line;
	int a;

	//Abertura do Arquivo
	txtRepository.open("EscalonamentoEntrada.txt");

	vector<int> vect;

	//Le linha por linha ate o fim do arquivo -> eof (end-of-file).
	while (!txtRepository.eof())
	{
		getline(txtRepository, line);
		stringstream ss(line);

		while (ss >> a)
		{
			vect.push_back(a); // Add um caractere ao vetor de int
			if (ss.peek() == ' ') // Irá verificar se o proximo caractere será um espaço
				ss.ignore();
		}
	}
	txtRepository.close();
	;
	/*
	for (int i = 0; i < vect.size(); i++) {

		z = vect[i];

		cout << z << endl;
	}
	*/



	n = vect[0];
	int* vectStart = new int[n];
	int* vectDuration = new int[n];
	int* vectPriority = new int[n];

	for (int i = 1, cont = 0; cont < n; i += 3, cont++) {

		vectPriority[cont] = vect[i];
		vectStart[cont] = vect[i + 1];
		vectDuration[cont] = vect[i + 2];
	}




	fifo(vectStart, vectDuration, vectPriority);

	prio(vectPriority, vectStart, vectDuration);

	srt(vectStart, vectDuration, vectPriority);

	rrq(vectStart, vectDuration, vectPriority);






	return 0;
}


