import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, accuracy_score
import matplotlib.pyplot as plt
import joblib

# Load processed stock data
data = pd.read_csv("data/processed_stock_data.csv")

# Drop any rows with missing values (if any)
data.dropna(inplace=True)

# Feature Columns
features = ['SMA', 'EMA', 'RSI', 'MACD', 'SignalLine']

# Add target variable: '1' for price increase, '0' for price decrease
data['Target'] = (data['Close'].shift(-1) > data['Close']).astype(int)

# Drop the last row as it will have no target value
data.dropna(inplace=True)

# Feature matrix (X) and target vector (y)
X = data[features]
y = data['Target']

# Split the data into training and testing sets (80% train, 20% test)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, shuffle=False)

# Standardize the features (scaling)
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Initialize the RandomForestClassifier model with some hyperparameters
model = RandomForestClassifier(n_estimators=100, random_state=42, max_depth=10, min_samples_split=5)

# Train the model using cross-validation (optional but helpful for better generalization)
cv_scores = cross_val_score(model, X_train_scaled, y_train, cv=5, scoring='accuracy')

print(f"Cross-validation scores: {cv_scores}")
print(f"Mean cross-validation accuracy: {cv_scores.mean():.4f}")

# Train model on the entire training set after cross-validation
model.fit(X_train_scaled, y_train)

# Make predictions on test set
y_pred = model.predict(X_test_scaled)

# Evaluate model's performance
accuracy = accuracy_score(y_test, y_pred)
print(f"Accuracy: {accuracy:.4f}")

# Display classification report
print("\nClassification Report:")
print(classification_report(y_test, y_pred, zero_division=0))

# Feature Importance Plot (Optional: helps in understanding model behavior)
feature_importances = model.feature_importances_
plt.figure(figsize=(10, 6))
plt.barh(features, feature_importances)
plt.xlabel('Feature Importance')
plt.title('Random Forest Feature Importance')
plt.show()

# Save the trained model
joblib.dump(model, "data/stock_price_prediction_model.pkl")
print("\nModel saved to disk as 'data/stock_price_prediction_model.pkl'")

# Final Prediction function (based on a new set of input features)
def final_prediction(input_data):
    """
    Predict whether it's a good purchase or not.
    Input: input_data (array-like, should contain values for ['SMA', 'EMA', 'RSI', 'MACD', 'SignalLine'])
    Output: 'Good Purchase' if predicted value is 1, 'Not a Good Purchase' if predicted value is 0
    """
    # Load the trained model (if not already in memory)
    model = joblib.load("data/stock_price_prediction_model.pkl")
    
    # Ensure the input_data is a pandas DataFrame with the correct feature names
    input_df = pd.DataFrame([input_data], columns=['SMA', 'EMA', 'RSI', 'MACD', 'SignalLine'])
    
    # Standardize the input data (same scaling as training data)
    input_scaled = scaler.transform(input_df)
    
    # Predict with the model
    prediction = model.predict(input_scaled)
    
    # Output decision based on prediction
    if prediction == 1:
        return "Good Purchase"
    else:
        return "Not a Good Purchase"

# Example of final prediction on a new set of features
# Replace the input values with actual values for your new stock data
new_input = [100, 105, 60, 0.02, 0.01]  # Example values for ['SMA', 'EMA', 'RSI', 'MACD', 'SignalLine']
decision = final_prediction(new_input)
print(f"Final Decision: {decision}")
