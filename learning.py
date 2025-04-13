import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split, cross_val_score
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, accuracy_score
import matplotlib.pyplot as plt
import joblib

# load and preprocess data
def load_and_preprocess_data(file_path):
    data = pd.read_csv(file_path)
    data.dropna(inplace=True)
    
    # Add target variable: '1' for price increase, '0' for price decrease
    data['Target'] = (data['Close'].shift(-1) > data['Close']).astype(int)
    data.dropna(inplace=True)
    return data

# split features and target variable
def split_features_and_target(data, features):
    X = data[features]
    y = data['Target']
    return X, y

# standardize features
def standardize_features(X_train, X_test):
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)
    return X_train_scaled, X_test_scaled, scaler

# train and evaluate the model
def train_and_evaluate_model(X_train, y_train, X_test, y_test):
    model = RandomForestClassifier(n_estimators=100, random_state=42, max_depth=10, min_samples_split=5)
    
    # Cross-validation
    cv_scores = cross_val_score(model, X_train, y_train, cv=5, scoring='accuracy')
    print(f"Cross-validation scores: {cv_scores}")
    print(f"Mean cross-validation accuracy: {cv_scores.mean():.4f}")
    
    # Train the model
    model.fit(X_train, y_train)
    
    # Test predictions
    y_pred = model.predict(X_test)
    accuracy = accuracy_score(y_test, y_pred)
    print(f"Test Accuracy: {accuracy:.4f}")
    print("\nClassification Report:")
    print(classification_report(y_test, y_pred, zero_division=0))
    
    return model

# plot feature importance
def plot_feature_importance(model, features):
    feature_importances = model.feature_importances_
    plt.figure(figsize=(10, 6))
    plt.barh(features, feature_importances)
    plt.xlabel('Feature Importance')
    plt.title('Random Forest Feature Importance')
    plt.show()

# save the trained model
def save_model(model, file_path):
    joblib.dump(model, file_path)
    print(f"\nModel saved to disk as '{file_path}'")

# final prediction function
def final_prediction(input_data, model, scaler, feature_names):
    input_df = pd.DataFrame([input_data], columns=feature_names)
    input_scaled = scaler.transform(input_df)
    prediction = model.predict(input_scaled)
    return "Good Purchase" if prediction == 1 else "Not a Good Purchase"

# Main
def main():
    # File path and features
    file_path = "data/processed_stock_data.csv"
    features = ['SMA', 'EMA', 'RSI', 'MACD', 'SignalLine']
    
    # Load and preprocess data
    data = load_and_preprocess_data(file_path)
    
    # Split data
    X, y = split_features_and_target(data, features)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, shuffle=False)
    
    # Standardize features
    X_train_scaled, X_test_scaled, scaler = standardize_features(X_train, X_test)
    
    # Train and evaluate model
    model = train_and_evaluate_model(X_train_scaled, y_train, X_test_scaled, y_test)
    
    # Plot feature importance
    plot_feature_importance(model, features)
    
    # Save model
    save_model(model, "data/stock_price_prediction_model.pkl")
    
    # Mean of features for prediction
    mean_values = data[features].mean().values
    decision = final_prediction(mean_values, model, scaler, features)
    print(f"Final Decision based on mean values: {decision}")

# Main
if __name__ == "__main__":
    main()