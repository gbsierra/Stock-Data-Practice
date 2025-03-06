import pandas as pd
import matplotlib.pyplot as plt

# Load raw stock data
df_raw = pd.read_csv('fetched_stock_data.csv')
df_raw['Date'] = pd.to_datetime(df_raw['Date'])

# Load processed stock data (w/ technical indicators)
df_processed = pd.read_csv('processed_stock_data.csv')
df_processed['Date'] = pd.to_datetime(df_processed['Date'])

# Set figure size
plt.figure(figsize=(12, 12))


# Plot raw stock prices (Open, Close, High, Low)
plt.subplot(3, 1, 1)  # 3 rows, 1 column, first plot
plt.plot(df_raw['Date'], df_raw['Open'], label='Raw Open', color='blue', alpha=0.7)
plt.plot(df_raw['Date'], df_raw['Close'], label='Raw Close', color='green', alpha=0.7)
plt.plot(df_raw['Date'], df_raw['High'], label='Raw High', color='red', alpha=0.7)
plt.plot(df_raw['Date'], df_raw['Low'], label='Raw Low', color='orange', alpha=0.7)
plt.title('Raw Stock Price Data')
plt.xlabel('Date')
plt.ylabel('Price')
plt.legend()


# Plot raw Volume (from fetched data)
plt.subplot(3, 1, 2)  # 3 rows, 1 column, second plot
plt.bar(df_raw['Date'], df_raw['Volume'], label='Raw Volume', color='gray', alpha=0.5)
plt.title('Raw Stock Volume')
plt.xlabel('Date')
plt.ylabel('Volume')


# Plot RSI from processed data
plt.subplot(3, 1, 3)  # 3 rows, 1 column, third plot
if 'RSI' in df_processed.columns:
    plt.plot(df_processed['Date'], df_processed['RSI'], label='RSI', color='brown', alpha=0.7)
    plt.axhline(45, color='red', linestyle='--', alpha=0.7, label='Overbought (70)')  # Overbought level
    plt.axhline(30, color='green', linestyle='--', alpha=0.7, label='Oversold (30)')  # Oversold level
plt.title('Relative Strength Index')
plt.xlabel('Date')
plt.ylabel('Indicator Value')
plt.legend()


# Adjust layout to avoid overlapping labels
plt.tight_layout()
# Display the plots
plt.show()
