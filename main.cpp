#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib> // For system()
#include "stockData.h"

// load stock data to fetched_stock_data.csv - generated by fetching_data.py
bool loadDataFromCSV(StockData& stock, const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;
    
    if (!file) {
        std::cerr << "Could not open the file: " << filepath << std::endl;
        return false;
    }

    // Skip the header line
    std::getline(file, line);

    // Read each line of the CSV and load data into the StockData object
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string date, open, close, high, low, volume;

        std::getline(ss, date, ',');
        std::getline(ss, open, ',');
        std::getline(ss, close, ',');
        std::getline(ss, high, ',');
        std::getline(ss, low, ',');
        std::getline(ss, volume, ',');

        stock.addDate(date);
        stock.addOpenPrice(std::stod(open));
        stock.addClosePrice(std::stod(close));
        stock.addHighPrice(std::stod(high));
        stock.addLowPrice(std::stod(low));
        stock.addVolume(std::stod(volume));
    }

    file.close();
    return true;
}


// Main Function
int main() {
    try {
        //Make a StockData instance with the symbol 'stockSymbol'
        std::string stockSymbol = "AAPL";
        StockData stock(stockSymbol);

        // Call the Python script to fetch stock data and save it as CSV
        std::string apiKey = "WQZC1XKXDIC9UADJ"; // API key
        std::string command = "python fetching_data.py " + stockSymbol + " " + apiKey;

        // Check if Python script executed successfully
        std::cout << "\nRunning Python script to fetch data...\n\n";
        int pythonScriptStatus = system(command.c_str());
        if (pythonScriptStatus != 0) {
            throw std::runtime_error("Error executing Python script fetching_data.py.");
        }
        //std::cout << "Python script executed successfully.\n";

        // Load stock data from CSV (the file created by fetching_data.py)
        std::string filepath = "data/fetched_stock_data.csv";
        if (!loadDataFromCSV(stock, filepath)) {
            throw std::runtime_error("Error loading stock data from CSV file.");
        }
        std::cout << "Stock data loading from " << filepath << ".\n";

        // Preprocess and calculate technical indicators
        std::cout << "Preprocessing and calculating technical indicators...\n";
        stock.preprocessData();
        std::cout << "Preprocessed data.\n";

        stock.calculateEMA(14);  // Hardcoded period for EMA
        std::cout << "Calculated EMA.\n";

        stock.calculateRSI(14);  // Hardcoded period for RSI
        std::cout << "Calculated RSI.\n";

        stock.calculateMACD();
        std::cout << "Calculated MACD.\n";

        stock.calculateBollingerBands(20, 2);  // Calculate Bollinger Bands with a multiplier of 2
        std::cout << "Calculated Bollinger Bands.\n";

        // Save the data to a CSV file
        if (!stock.saveProcessedDataToCSV()) {
            throw std::runtime_error("Error saving processed stock data to CSV.");
        }
        std::cout << "Success! " << stockSymbol << " data processed and saved to processed_stock_data.csv!\n\n";

        // After saving data, call Python script for visualization
        std::cout << "Now, running Python script to visualize raw data...\n";
        int graphScriptStatus = system("python graphing_data.py");
        if (graphScriptStatus != 0) {
            throw std::runtime_error("Error executing Python script graphing_data.py.");
        }

        // then, call Python script for prediction
        std::cout << "Now, running Python script to make prediction...\n\n";
        int learningScriptStatus = system("python learning.py");
        if (learningScriptStatus != 0) {
            throw std::runtime_error("Error executing Python script learning.py.");
        }

        std::cout << "\nData processed and visualized successfully.\n";

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;  // indicates failure
    }

    return 0;  // indicates success
}