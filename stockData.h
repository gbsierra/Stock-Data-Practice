#ifndef STOCKDATA_H
#define STOCKDATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>

class StockData {
private:
    std::string symbol;
    std::vector<std::string> dates;
    std::vector<double> openPrices, closePrices, highPrices, lowPrices, volumes;
    std::vector<double> simpleMovingAverage, exponentialMovingAverage, rsi, macdLine, signalLine;

    // Helper method for single EMA (Exponential Moving Average)
    std::vector<double> calculateSingleEMA(const std::vector<double>& prices, int period);

public:
    // Constructor
    StockData(const std::string& symbol);

    // Getter methods
    std::vector<double> getPrices() const;
    std::string getSymbol() const;
    std::vector<std::string> getDates() const;
    std::vector<double> getOpenPrices() const;
    std::vector<double> getClosePrices() const;
    std::vector<double> getHighPrices() const;
    std::vector<double> getLowPrices() const;
    std::vector<double> getVolume() const;
    std::vector<double> getSimpleMovingAverage() const;
    std::vector<double> getExponentialMovingAverage() const;
    std::vector<double> getRSI() const;
    std::vector<double> getMACDLine() const;
    std::vector<double> getSignalLine() const;

    // Mutators
    void setSymbol(const std::string& symbol);
    void addDate(const std::string& date);
    void addOpenPrice(double price);
    void addClosePrice(double price);
    void addHighPrice(double price);
    void addLowPrice(double price);
    void addVolume(double volume);

    // Methods to load and preprocess data
    void loadHistoricalPrices(const std::string& filepath);
    void preprocessData(); // Implement any preprocessing if needed
    void calculateBollingerBands(int period, double multiplier);

    // Methods to calculate indicators
    void calculateEMA(int period);  // Calculate Exponential Moving Average
    void calculateRSI(int period);  // Calculate Relative Strength Index
    void calculateMACD();           // Calculate Moving Average Convergence Divergence
    void calculateSMA(int period);  // Calculate Simple Moving Average

    // Methods to plot data
    void plotStockPrices() const;
    void plotIndicators() const;

    // Data normalization
    void normalizeData();
    bool saveProcessedDataToCSV();

};

#endif // STOCKDATA_H
