#include "stockData.h"
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <cmath>

// Constructor
StockData::StockData(const std::string &symbol) : symbol(symbol) {}

// Accessors
std::vector<double> StockData::getPrices() const {
    return closePrices; // Returning close prices as a default for "prices"
}

std::string StockData::getSymbol() const {
    return symbol;
}

std::vector<std::string> StockData::getDates() const {
    return dates;
}

std::vector<double> StockData::getOpenPrices() const {
    return openPrices;
}

std::vector<double> StockData::getClosePrices() const {
    return closePrices;
}

std::vector<double> StockData::getHighPrices() const {
    return highPrices;
}

std::vector<double> StockData::getLowPrices() const {
    return lowPrices;
}

std::vector<double> StockData::getVolume() const {
    return volumes;
}

std::vector<double> StockData::getSimpleMovingAverage() const {
    std::vector<double> sma;
    int period = 20; // Assuming you want a 20-period SMA

    if (closePrices.size() < period) {
        std::cerr << "Error: Not enough data to calculate SMA. Data size: " 
                  << closePrices.size() << ", period: " << period << std::endl;
        return sma;
    }

    for (size_t i = period - 1; i < closePrices.size(); ++i) {
        double sum = 0.0;
        for (size_t j = i - period + 1; j <= i; ++j) {
            sum += closePrices[j];
        }
        sma.push_back(sum / period);
    }

    return sma;
}

std::vector<double> StockData::getExponentialMovingAverage() const {
    return exponentialMovingAverage;
}

std::vector<double> StockData::getRSI() const {
    return rsi;
}

std::vector<double> StockData::getMACDLine() const {
    return macdLine;
}

std::vector<double> StockData::getSignalLine() const {
    return signalLine;
}


// Mutators
void StockData::addDate(const std::string& date) {
    dates.push_back(date);
}

void StockData::addOpenPrice(double price) {
    openPrices.push_back(price);
}

void StockData::addClosePrice(double price) {
    closePrices.push_back(price);
}

void StockData::addHighPrice(double price) {
    highPrices.push_back(price);
}

void StockData::addLowPrice(double price) {
    lowPrices.push_back(price);
}

void StockData::addVolume(double volume) {
    volumes.push_back(volume);
}

// Functions 
//-----------------
// calculate single moving average
void StockData::calculateSMA(int period)
{
    simpleMovingAverage.clear();
    for (size_t i = 0; i <= closePrices.size() - period; ++i)
    {
        double sum = std::accumulate(closePrices.begin() + i, closePrices.begin() + i + period, 0.0);
        simpleMovingAverage.push_back(sum / period);
    }
}

// helper method for EMA and MACD
std::vector<double> StockData::calculateSingleEMA(const std::vector<double>& prices, int period)
{
    std::vector<double> ema;
    if (prices.empty()) return ema;

    double multiplier = 2.0 / (period + 1);
    ema.push_back(prices[0]);

    for (size_t i = 1; i < prices.size(); ++i)
    {
        double value = (prices[i] - ema.back()) * multiplier + ema.back();
        ema.push_back(value);
    }

    return ema;
}

// calculate exponential moving average
void StockData::calculateEMA(int period)
{
    if (closePrices.empty()) return; // Early return if no prices

    exponentialMovingAverage.clear();
    double multiplier = 2.0 / (period + 1);
    exponentialMovingAverage.push_back(closePrices[0]);

    for (size_t i = 1; i < closePrices.size(); ++i)
    {
        double ema = (closePrices[i] - exponentialMovingAverage.back()) * multiplier + exponentialMovingAverage.back();
        exponentialMovingAverage.push_back(ema);
    }
}

// calculate Relative Strength Index (RSI)
void StockData::calculateRSI(int period)
{
    rsi.clear();
    if (closePrices.size() < period) return; // Ensure there are enough prices

    // Initial gains and losses for the first period
    std::vector<double> gains(period, 0.0), losses(period, 0.0);
    for (size_t i = 1; i <= period; ++i)
    {
        double change = closePrices[i] - closePrices[i - 1];
        if (change > 0)
            gains[i - 1] = change;
        else
            losses[i - 1] = -change;
    }

    double avgGain = std::accumulate(gains.begin(), gains.end(), 0.0) / period;
    double avgLoss = std::accumulate(losses.begin(), losses.end(), 0.0) / period;
    double rs = (avgLoss == 0.0) ? 0.0 : avgGain / avgLoss;
    rsi.push_back(100.0 - (100.0 / (1.0 + rs)));

    // Calculate RSI for the remaining data points
    for (size_t i = period; i < closePrices.size(); ++i)
    {
        double change = closePrices[i] - closePrices[i - 1];
        double gain = (change > 0) ? change : 0.0;
        double loss = (change < 0) ? -change : 0.0;

        avgGain = (avgGain * (period - 1) + gain) / period;
        avgLoss = (avgLoss * (period - 1) + loss) / period;

        rs = (avgLoss == 0.0) ? 0.0 : avgGain / avgLoss;
        rsi.push_back(100.0 - (100.0 / (1.0 + rs)));
    }
}

// calculate moving average convergence/divergence
void StockData::calculateMACD()
{
    int shortPeriod = 12, longPeriod = 26, signalPeriod = 9;

    // Calculate short-period and long-period EMA
    std::vector<double> shortEMA = calculateSingleEMA(closePrices, shortPeriod);
    std::vector<double> longEMA = calculateSingleEMA(closePrices, longPeriod);

    // Calculate MACD Line (difference between short-term and long-term EMAs)
    macdLine.clear();
    for (size_t i = 0; i < closePrices.size(); ++i)
    {
        macdLine.push_back(shortEMA[i] - longEMA[i]);
    }

    // Calculate Signal Line (EMA of MACD line)
    signalLine = calculateSingleEMA(macdLine, signalPeriod);
}

// calculate bollinger bands
void StockData::calculateBollingerBands(int period, double multiplier) {
    // Get the SMA values using the getSimpleMovingAverage function
    std::vector<double> sma = getSimpleMovingAverage();

    // Ensure the SMA vector has enough data
    if (sma.size() < period) {
        std::cerr << "Error: Not enough data to calculate Bollinger Bands. SMA size: " 
                  << sma.size() << ", period: " << period << std::endl;
        return;
    }

    // Declare vectors for Upper and Lower Bollinger Bands
    std::vector<double> upperBand;
    std::vector<double> lowerBand;

    // Calculate Bollinger Bands using the SMA
    for (size_t i = period - 1; i < sma.size(); ++i) {
        double sum = 0.0;

        // Calculate Standard Deviation based on the SMA (using closePrices, not sma[i] directly)
        for (size_t j = i - period + 1; j <= i; ++j) {
            sum += std::pow(closePrices[j] - sma[j - period + 1], 2); // Variance based on SMA
        }

        double stdDev = std::sqrt(sum / period);  // Standard deviation

        // Calculate Upper and Lower Bollinger Bands
        upperBand.push_back(sma[i] + (stdDev * multiplier));
        lowerBand.push_back(sma[i] - (stdDev * multiplier));
    }

    /*
    // Optional: Print the calculated Bollinger Bands for demonstration
    std::cout << "Date, SMA, Upper Band, Lower Band\n";
    for (size_t i = period - 1; i < sma.size(); ++i) {
        std::cout << dates[i] << ", "
                  << sma[i] << ", "
                  << upperBand[i - period + 1] << ", "
                  << lowerBand[i - period + 1] << "\n";
    }
    */

    // If you want to save these values as member variables, uncomment the following:
    // this->upperBand = upperBand;
    // this->lowerBand = lowerBand;
}

// normalize data
void StockData::normalizeData()
{
    auto normalize = [](std::vector<double>& data)
    {
        double minVal = *std::min_element(data.begin(), data.end());
        double maxVal = *std::max_element(data.begin(), data.end());
        for (auto& value : data)
        {
            value = (value - minVal) / (maxVal - minVal);
        }
    };

    normalize(openPrices);
    normalize(closePrices);
    normalize(highPrices);
    normalize(lowPrices);
    normalize(volumes);
}

// remove invalid data and normalizes
void StockData::preprocessData()
{
    // Remove rows with missing data (e.g., if any price is negative or zero)
    for (size_t i = 0; i < closePrices.size(); ++i)
    {
        if (closePrices[i] <= 0 || openPrices[i] <= 0 || highPrices[i] <= 0 || lowPrices[i] <= 0 || volumes[i] <= 0)
        {
            // skip or remove an entry if it has invalid data
            dates.erase(dates.begin() + i);
            openPrices.erase(openPrices.begin() + i);
            closePrices.erase(closePrices.begin() + i);
            highPrices.erase(highPrices.begin() + i);
            lowPrices.erase(lowPrices.begin() + i);
            volumes.erase(volumes.begin() + i);
            --i;  // adjust the index as we removed an element
        }
    }

    normalizeData();
}

// saving data to csv
bool StockData::saveProcessedDataToCSV() {
    std::ofstream file("data/processed_stock_data.csv");
    if(!file){
        return false;
    }
    file << "Date,Open,High,Low,Close,Volume,SMA,EMA,RSI,MACD,SignalLine\n";
    
    size_t dataSize = getClosePrices().size();
    
    for (size_t i = 0; i < dataSize; ++i) {
        file << getDates()[i] << ","
             << getOpenPrices()[i] << ","
             << getHighPrices()[i] << ","
             << getLowPrices()[i] << ","
             << getClosePrices()[i] << ","
             << getVolume()[i] << ","
             << (i >= 14 ? getSimpleMovingAverage()[i] : 0) << ","
             << (i >= 14 ? getExponentialMovingAverage()[i] : 0) << ","
             << (i >= 14 ? getRSI()[i] : 0) << ","
             << (i >= 14 ? getMACDLine()[i] : 0) << ","
             << (i >= 14 ? getSignalLine()[i] : 0) << "\n";
    }
    file.close();
    return true;
}