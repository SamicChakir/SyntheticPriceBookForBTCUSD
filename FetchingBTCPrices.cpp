// FetchingBTCPrices.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "Helpers.h"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

using namespace web::http::client;



int main()
{
	std::cout << "Fetching Bitcoin Price From Coinbase !\n";



	// create http_client to send the request.
	http_client client(U("https://api.exchange.coinbase.com/products/"));
	// build request uri and start the request.
	uri_builder builder(U(""));
	builder.append_path(U("BTC-USD/book?level=2"));

	// Make http request.
	pplx::task<void> requestTask = client.request(methods::GET, builder.to_string()).then([=](http_response response)
		{
			if (response.status_code() == status_codes::OK) {
				printf("received response From Coinbase status code:%u\n", response.status_code());
				return  response.extract_json();
			}
			throw std::runtime_error("Response not OK");

		})
		.then([=](json::value jsonQueryResponse) {
			auto bestPricesFromLevel2MarketData = Helpers::getBidAndAskPrices(jsonQueryResponse);
			std::string providerC = "Coinbase";
			Helpers::displayPricesPerQuantities(bestPricesFromLevel2MarketData, std::cout, providerC);

			});

		http_client clientBitStamp(U("https://www.bitstamp.net/api/v2/"));
		// build request uri and start the request.
		uri_builder builderBitStamp(U(""));
		builderBitStamp.append_path(U("order_book/btcusd/"));
		pplx::task<void> requestTaskBitStamp = clientBitStamp.request(methods::GET, builderBitStamp.to_string()).then([=](http_response bitStampResponse)
			{
				if (bitStampResponse.status_code() == status_codes::OK) {
					printf("received response BitStamp status code:%u\n", bitStampResponse.status_code());
					return  bitStampResponse.extract_json();
				}
				throw std::runtime_error("Response not OK");

			})
			.then([=](json::value jsonQueryBitStampResponse) {
				std::string providerB = "BitStamp";
				auto bestPricesFromLevel2MarketDataBitStamp = Helpers::getBidAndAskPrices(jsonQueryBitStampResponse);
				Helpers::displayPricesPerQuantities(bestPricesFromLevel2MarketDataBitStamp, std::cout, providerB);
				});



			// wait for all the outstanding i/o to complete and handle any exceptions
			try
			{
				requestTask.wait();
				requestTaskBitStamp.wait();
				system("pause");

			}
			catch (const std::exception& e)
			{
				printf("error exception:%s\n", e.what());
			}

			return 0;

}

