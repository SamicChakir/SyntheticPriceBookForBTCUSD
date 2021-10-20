#ifndef HELPERS_H
#define HELPERS_H

#include<map>
#include<string>
#include <cpprest/http_client.h>
using namespace web;
using namespace utility;

class Helpers
{
public:
	static std::map<string_t, std::map<double, double>> getBidAndAskPrices(json::value &jsonPrices);
	static std::map<double, double> getBidPrices(json::value& jsonPrices, string_t &bidName);
	static std::map<double, double> getAskPrices(json::value& jsonPrices, string_t &askName);
	static void displayPricesPerQuantities(std::map<string_t, std::map<double, double>>& prices, std::ostream& os, std::string &provider);
private:
	Helpers() {}
};

#endif //HELPERS_H