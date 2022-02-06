#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <chrono>
void citire(std::vector<std::vector<int>>& listaDeAdiacenta, const std::string& fisier) {

	for (std::ifstream f(fisier); !f.eof();) {
		std::string aux, aux2;
		getline(f, aux);
		std::istringstream iss(aux);
		int nod, copii;
		iss >> nod;
		listaDeAdiacenta.resize(listaDeAdiacenta.size() + 1);
		while (iss >> copii) {
			listaDeAdiacenta[nod - 1].push_back(copii - 1);
		}
	}
}
void dfs(const std::vector<std::vector<int>>& listaDeAdiacenta, std::vector<int>& esteVizitat, std::stack<int>& sortare, std::stack<int>& stiva, int batch, int& ciclu) {
	if (listaDeAdiacenta[stiva.top()].size() == 0) {
		sortare.push(stiva.top());
		esteVizitat[stiva.top()] = batch;
		stiva.pop();
	}
	else {
		int copii = 0;
		esteVizitat[stiva.top()] = batch;
		for (int i = 0; i < listaDeAdiacenta[stiva.top()].size(); i++) {
			if (esteVizitat[listaDeAdiacenta[stiva.top()][i]] == batch) {
				for (int j = 0; j < listaDeAdiacenta[listaDeAdiacenta[stiva.top()][i]].size(); j++) {
					if (esteVizitat[listaDeAdiacenta[stiva.top()][i]] == batch) {
						ciclu = 1;
					}
				}
			}
			if (esteVizitat[listaDeAdiacenta[stiva.top()][i]] == 0) {
				esteVizitat[listaDeAdiacenta[stiva.top()][i]] = batch;
				stiva.push(listaDeAdiacenta[stiva.top()][i]);
				dfs(listaDeAdiacenta, esteVizitat, sortare, stiva, batch, ciclu);
			}
		}
		sortare.push(stiva.top());
		stiva.pop();
	}

}
void sortareTopologica(const std::vector<std::vector<int>>& listaDeAdiacenta) {
	std::stack<int> sortare;
	std::stack<int> stiva;
	std::vector<int> esteVizitat;
	for (int i = 0; i < listaDeAdiacenta.size(); i++) {
		esteVizitat.push_back(0);
	}
	int toateSuntVizitate = 0;
	int batch = 1, ciclu = 0;
	while (!toateSuntVizitate) {
		int nodCurent = -1;
		for (int i = 0; i < esteVizitat.size() && nodCurent == -1; i++) {
			if (esteVizitat[i] == 0) {
				nodCurent = i;
				esteVizitat[i] = batch;
			}
		}
		if (nodCurent == -1) {
			toateSuntVizitate = 1;
		}
		else {
			stiva.push(nodCurent);
			dfs(listaDeAdiacenta, esteVizitat, sortare, stiva, batch, ciclu);
		}
		batch++;
	}
	if (ciclu) {
		std::cout << "Exista ciclu!";
	}
	else {
		while (!sortare.empty()) {
			std::cout << sortare.top() + 1 << ' ';
			sortare.pop();
		}
	}
}
int main()
{
	std::vector<std::vector<int>> listaDeAdiacenta;
	citire(listaDeAdiacenta, "Lista de adiacenta.txt");
	auto started = std::chrono::high_resolution_clock::now();
	sortareTopologica(listaDeAdiacenta);
	auto done = std::chrono::high_resolution_clock::now();
	std::cout << "\nTimpul de sortare: " << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << " ms";
}