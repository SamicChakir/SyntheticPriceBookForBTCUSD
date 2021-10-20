#include "Helpers.h"
#include <boost/lexical_cast.hpp>
#include<iomanip>


std::map<string_t, std::map<double, double>> Helpers::getBidAndAskPrices(json::value& jsonPrices) {
	std::map<string_t, std::map< double, double>> bestpricesperquantity;
	string_t bid = L"bids";
	string_t ask = L"asks";

	bestpricesperquantity[L"bids"] = getBidPrices(jsonPrices, bid);
	bestpricesperquantity[L"asks"] = getAskPrices(jsonPrices, ask);
	return bestpricesperquantity;
}

std::map<double, double> Helpers::getBidPrices(json::value& jsonPrices, string_t& bidName)
{
	auto sidePrices = jsonPrices.at(bidName).as_array();
	auto q4 = 0.0001;
	auto q3 = 1.0;
	auto q2 = 5.0;
	auto q1 = 10.0;
	std::map< double, double> pricePerQuantity = { {q1, -DBL_MAX }, {q2, -DBL_MAX }, {q3, -DBL_MAX },  {q4, -DBL_MAX } };;


	for (auto iter = sidePrices.cbegin(); iter != sidePrices.cend(); ++iter) {
		auto priceWideString = iter->at(0).serialize();
		priceWideString = priceWideString.substr(1, priceWideString.size() - 2);
		double priceDouble = boost::lexical_cast<double>(priceWideString);

		auto qtyWideString = iter->at(1).serialize();
		qtyWideString = qtyWideString.substr(1, qtyWideString.size() - 2);
		double quantityDouble = boost::lexical_cast<double>(qtyWideString);

		if (quantityDouble >= 10) {
			pricePerQuantity[q1] = std::max(pricePerQuantity[q1], priceDouble);
			pricePerQuantity[q2] = std::max(pricePerQuantity[q2], priceDouble);
			pricePerQuantity[q3] = std::max(pricePerQuantity[q3], priceDouble);
			pricePerQuantity[q4] = std::max(pricePerQuantity[q4], priceDouble);
		}
		else if (quantityDouble >= 5) {
			pricePerQuantity[q2] = std::max(pricePerQuantity[q2], priceDouble);
			pricePerQuantity[q3] = std::max(pricePerQuantity[q3], priceDouble);
			pricePerQuantity[q4] = std::max(pricePerQuantity[q4], priceDouble);
		}
		else if (quantityDouble >= 1) {
			pricePerQuantity[q3] = std::max(pricePerQuantity[q3], priceDouble);
			pricePerQuantity[q4] = std::max(pricePerQuantity[q4], priceDouble);
		}
		else if (quantityDouble >= 0.0001) {
			pricePerQuantity[q4] = std::max(pricePerQuantity[q4], priceDouble);
		}

	}

	return pricePerQuantity;
}

std::map<double, double> Helpers::getAskPrices(json::value& jsonPrices, string_t& bidName)
{
	auto sidePrices = jsonPrices.at(bidName).as_array();
	auto q4 = 0.0001;
	auto q3 = 1.0;
	auto q2 = 5.0;
	auto q1 = 10.0;
	std::map< double, double> pricePerQuantity = { {q1, DBL_MAX }, {q2, DBL_MAX }, {q3, DBL_MAX },  {q4, DBL_MAX } };;


	for (auto iter = sidePrices.cbegin(); iter != sidePrices.cend(); ++iter) {
		auto priceWideString = iter->at(0).serialize();
		priceWideString = priceWideString.substr(1, priceWideString.size() - 2);
		double priceDouble = boost::lexical_cast<double>(priceWideString);

		auto qtyWideString = iter->at(1).serialize();
		qtyWideString = qtyWideString.substr(1, qtyWideString.size() - 2);
		double quantityDouble = boost::lexical_cast<double>(qtyWideString);

		if (quantityDouble >= 10) {
			pricePerQuantity[q1] = std::min(pricePerQuantity[q1], priceDouble);
			pricePerQuantity[q2] = std::min(pricePerQuantity[q2], priceDouble);
			pricePerQuantity[q3] = std::min(pricePerQuantity[q3], priceDouble);
			pricePerQuantity[q4] = std::min(pricePerQuantity[q4], priceDouble);
		}
		else if (quantityDouble >= 5) {
			pricePerQuantity[q2] = std::min(pricePerQuantity[q2], priceDouble);
			pricePerQuantity[q3] = std::min(pricePerQuantity[q3], priceDouble);
			pricePerQuantity[q4] = std::min(pricePerQuantity[q4], priceDouble);
		}
		else if (quantityDouble >= 1) {
			pricePerQuantity[q3] = std::min(pricePerQuantity[q3], priceDouble);
			pricePerQuantity[q4] = std::min(pricePerQuantity[q4], priceDouble);
		}
		else if (quantityDouble >= 0.0001) {
			pricePerQuantity[q4] = std::min(pricePerQuantity[q4], priceDouble);
		}

	}

	return pricePerQuantity;
}



void Helpers::displayPricesPerQuantities(std::map<string_t, std::map<double, double>>& prices, std::ostream& os, std::string& provider) {
	std::cout << " This is " << provider << " pricebook" << std::endl;
	int colWidth = 15;
	os << std::setfill('*') << std::setw(5 * colWidth) << "*" << std::endl;
	os << std::setfill(' ') << std::fixed;
	os << std::setw(colWidth) << "Quantity" << std::setw(colWidth) << "AskPrice" << std::setw(colWidth) << "BidPrice" << std::setw(colWidth) << "Level" << std::setw(colWidth) << "Provider" << std::endl;
	os << std::setfill('*') << std::setw(5 * colWidth) << "*" << std::endl;
	os << std::setfill(' ') << std::fixed;

	os << std::setw(colWidth) << 10.0 << std::setw(colWidth) << prices[L"asks"][10.0] << std::setw(colWidth) << prices[L"bids"][10.0] << std::setw(colWidth) << 2 << std::setw(colWidth) << provider << std::endl;
	os << std::setw(colWidth) << 5.0 << std::setw(colWidth) << prices[L"asks"][5.0] << std::setw(colWidth) << prices[L"bids"][5.0] << std::setw(colWidth) << 2 << std::setw(colWidth) << provider << std::endl;
	os << std::setw(colWidth) << 1.0 << std::setw(colWidth) << prices[L"asks"][1.0] << std::setw(colWidth) << prices[L"bids"][1.0] << std::setw(colWidth) << 2 << std::setw(colWidth) << provider << std::endl;
	os << std::setw(colWidth) << 0.0001 << std::setw(colWidth) << prices[L"asks"][0.0001] << std::setw(colWidth) << prices[L"bids"][0.0001] << std::setw(colWidth) << 2 << std::setw(colWidth) << provider << std::endl;

}