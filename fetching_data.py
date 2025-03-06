# fetch_stock_data.py

import requests
import pandas as pd
import sys

# Fetch stock data from Alpha Vantage API
def fetch_stock_data(symbol: str, api_key: str):
    url = f"https://www.alphavantage.co/query"
    params = {
        'function': 'TIME_SERIES_DAILY',
        'symbol': symbol,
        'apikey': api_key
    }
    
    response = requests.get(url, params=params)
    
    if response.status_code == 200:
        data = response.json()
        return data
    else:
        print(f"Error fetching data for {symbol}: {response.status_code}")
        return None

# Parse stock data and save it to CSV
def parse_and_save_data(symbol: str, api_key: str, filename="fetched_stock_data.csv"):
    data = fetch_stock_data(symbol, api_key)
    if data and 'Time Series (Daily)' in data:
        df = pd.DataFrame.from_dict(data['Time Series (Daily)'], orient='index')
        df = df.rename(columns={
            '1. open': 'Open',
            '2. high': 'High',
            '3. low': 'Low',
            '4. close': 'Close',
            '5. volume': 'Volume'
        })
        df.index = pd.to_datetime(df.index)
        df = df[['Open', 'High', 'Low', 'Close', 'Volume']]
        df = df.sort_index(ascending=True)  # Sort by date (ascending)
        
        # Reset index to include Date as a column
        df.reset_index(inplace=True)
        df.rename(columns={'index': 'Date'}, inplace=True)  # Rename index to 'Date'
        
        df.to_csv(filename, index=False)  # Save CSV without writing index
        print(f"Success! {symbol} stock data saved to {filename}\n")
    else:
        print(f"No valid data available for {symbol}.")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python fetch_stock_data.py <stock_symbol> <api_key>")
        sys.exit(1)
    
    stock_symbol = sys.argv[1]
    api_key = sys.argv[2]
    
    parse_and_save_data(stock_symbol, api_key)
