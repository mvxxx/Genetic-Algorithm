#include "Simulator.hpp"


void Simulator::createNewPopulation(int8_t chromosomesAmmount, int8_t genesAmmount)
{
	tempPopulation.clear();
	population.clear();

	while (chromosomesAmmount--)
	{
		population.emplace_back();
		for (uint16_t i = 0; i < genesAmmount; i++) population[population.size() - 1].first.push_back(rand() % 100 >= 50);
	}
}

std::vector<std::pair<std::vector<bool>, float>> Simulator::getPopulation()
{
	return population;
}

void Simulator::randomGenes()
{
	for (auto& chromosomes : population)
		for (auto& genes : chromosomes.first)
			genes = rand() % 100 >= 50;
}

void Simulator::setAdaption()
{
	firstStageAdaptation();
	secondStageAdaptation(countAdaptation());
	thirdStageAdaptation();
}

void Simulator::firstStageAdaptation()
{
	for (auto& var : population)
		var.second = std::count(var.first.begin(), var.first.end(), true);
}

int Simulator::countAdaptation()
{
	auto sum = 0;

	for (auto& var : population)
		sum += var.second;

	return sum;
}

void Simulator::secondStageAdaptation(int sumAdaptation)
{
	for (auto& var : population)
		var.second = var.second / (float)sumAdaptation;
}

void Simulator::thirdStageAdaptation()
{
	for (size_t i = 1; i < population.size(); i++)
		population[i].second += population[i-1].second;
	
}

void Simulator::chooseChromosomes()
{
	std::vector<uint8_t> randomNumbers;

	for (size_t i = 0; i < population.size(); i++)
		randomNumbers.push_back(rand()%100);
	
	for (auto&number : randomNumbers)
	{
		for (size_t i = 0; i < population.size(); i++)
			if (number < population[i].second)
				tempPopulation.push_back(population[i-1].first);
	}
}

void Simulator::crossPopulation()
{
	for (size_t i = 0; i < tempPopulation.size(); i+=2)
		crossPair(std::pair<std::vector<bool>&, std::vector<bool>&>(tempPopulation[i], tempPopulation[i + 1]));
	
}

void Simulator::crossPair(std::pair<std::vector<bool>&, std::vector<bool>&> chromosomes)
{
	auto crossoverProbability = (rand() % 100)/100.f;

	if (crossoverProbability < crossoverProbability)
		return;

	auto firstCopy = chromosomes.first;

	float lk = rand()%chromosomes.first.size();

	for (int8_t i = lk; i < chromosomes.first.size(); i++)
		chromosomes.first[i] = chromosomes.second[i];

	for (int8_t i = lk; i < chromosomes.first.size(); i++)
		chromosomes.second[i] = firstCopy[i];
	
}

void Simulator::mutatePopulation()
{
	float p_m;

	for (auto & chromosome : tempPopulation)
	{
		for (auto & gene : chromosome)
		{
			p_m = (rand() % 100) / 100.f;
			
			if (p_m < mutationProbability)
				gene = !gene;
		}
	}

}

void Simulator::confirmNewPopulation()
{
	for (size_t i = 0; i < population.size(); i++)
		population[i].first = tempPopulation[i];
	
	setAdaption();
}

Simulator::Simulator(int8_t chromosomesAmmount, int8_t genesAmmount, float p_c, float p_m)
	:mutationProbability(p_m),crossoverProbability(p_c)
{
		createNewPopulation(chromosomesAmmount,genesAmmount);
}

void  Simulator::simulate()
{
	srand(time(NULL));

	setAdaption();
	chooseChromosomes();
	crossPopulation();
	mutatePopulation();
	confirmNewPopulation();
}
