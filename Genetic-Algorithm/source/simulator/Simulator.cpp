#include "Simulator.hpp"


void Simulator::createNewPopulation(int chromosomesAmmount, int genesAmmount)
{
	population.clear();

	while (chromosomesAmmount--)
	{
		population.emplace_back();
		for (uint16_t i = 0; i < genesAmmount; i++) population[population.size() - 1].first.emplace_back(rand() % 100 >= 50);
	}
}

std::vector<std::pair<std::vector<bool>, float>> Simulator::getPopulation()
{
	return population;
}

std::vector<std::vector<bool>> Simulator::getHistory(int number)
{
	return history[number];
}

int Simulator::getHistorySize()
{
	return history.size();
}

void Simulator::setPropeties(int chromosomesAmmount, int genesAmmount, float p_c, float p_m)
{
	crossoverProbability = p_c;
	mutationProbability = p_m;
	createNewPopulation(chromosomesAmmount, genesAmmount);
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
		var.second /= (float)sumAdaptation;
}

void Simulator::thirdStageAdaptation()
{
	for (size_t i = 1; i < population.size(); i++)
		population[i].second += population[i - 1].second;
}

void Simulator::chooseChromosomes(std::vector<std::vector<bool>>& tempPopulation)
{
	std::vector<int> randomNumbers;

	for (size_t i = 0; i < population.size(); i++)
		randomNumbers.emplace_back(rand() % 100);

	for (auto&number : randomNumbers)
	{
		for (size_t i = 0; i < population.size(); i++)
		{
			if (number < population[i].second * 100)
			{
				tempPopulation.emplace_back(population[i].first);
				break;
			}
		}

	}
}

void Simulator::crossPopulation(std::vector<std::vector<bool>>& tempPopulation)
{
	for (size_t i = 0; i < tempPopulation.size(); i += 2)
		crossPair(std::pair<std::vector<bool>&, std::vector<bool>&>(tempPopulation[i], tempPopulation[i + 1]));

}

void Simulator::crossPair(std::pair<std::vector<bool>&, std::vector<bool>&> chromosomes)
{
	auto crossoverProb = (rand() % 100) / 100.f;

	if (crossoverProb < crossoverProbability)
		return;

	auto firstCopy = chromosomes.first;

	int lk = rand() % chromosomes.first.size();

	for (int8_t i = lk; i < chromosomes.first.size(); i++)
		chromosomes.first[i] = chromosomes.second[i];

	for (int8_t i = lk; i < chromosomes.first.size(); i++)
		chromosomes.second[i] = firstCopy[i];

}

void Simulator::mutatePopulation(std::vector<std::vector<bool>>& tempPopulation)
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

void Simulator::confirmNewPopulation(std::vector<std::vector<bool>>& tempPopulation)
{
	for (size_t i = 0; i < population.size(); i++)
		population[i].first = tempPopulation[i];

	setAdaption();
}

Simulator::Simulator(int chromosomesAmmount, int genesAmmount, float p_c, float p_m)
	:mutationProbability(p_m), crossoverProbability(p_c)
{
	createNewPopulation(chromosomesAmmount, genesAmmount);
}

void Simulator::simulate(int generations)
{
	srand(time(NULL));
	
	history.clear();

	for (size_t i = 0; i < generations; i++)
	{
		//Population { chromosomes/Adaptation)
		std::vector<std::vector<bool>> tempPopulation;

		setAdaption();
		chooseChromosomes(tempPopulation);
		crossPopulation(tempPopulation);
		mutatePopulation(tempPopulation);
		confirmNewPopulation(tempPopulation);

		history.push_back(tempPopulation);
	}
}