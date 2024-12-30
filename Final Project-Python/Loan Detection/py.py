from flask import Flask, render_template, request
import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score
import joblib

app = Flask(__name__)

# Loading from a CSV file
def load_data():
    data = pd.read_csv('loan_data.csv')  # Use the newly generated CSV file
    return data

# Preprocess the data
def preprocess_data(data):
    # Handle missing values
    data.fillna(0, inplace=True)  # If there are any missing values, fill with 0
    # Calculate debt-to-income ratio if not present
    if 'debt_to_income' not in data.columns:
        data['debt_to_income'] = data['debt'] / data['income']  # Use actual debt
    return data

# Train the model
def train_model(data):
    # Preprocess the data first
    processed_data = preprocess_data(data)

    # Define features (X) and target variable (y)
    X = processed_data[['income', 'credit_score', 'debt_to_income', 'age']]  # Added 'age' as a feature
    y = processed_data['loan_approved']  # Target variable (1 = Approved, 0 = Denied)

    # Split the data into training and testing sets (80% training, 20% testing)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

    # Train a Random Forest Classifier with hyperparameters
    model = RandomForestClassifier(n_estimators=200, max_depth=20, max_features='sqrt',
                                   min_samples_split=10, min_samples_leaf=4, bootstrap=True)
    model.fit(X_train, y_train)

    # Evaluate the model
    predictions = model.predict(X_test)
    accuracy = accuracy_score(y_test, predictions)
    print(f'Model Accuracy: {accuracy:.2f}')  # Display accuracy

    # Save the trained model for later use
    joblib.dump(model, 'loan_model.pkl')

# Loading the trained model
def load_model():
    return joblib.load('loan_model.pkl')

# Web application routing
@app.route('/', methods=['GET', 'POST'])
def index():
    result = ""
    error = ""
    if request.method == 'POST':
        try:
            # Get input values from the form
            income = float(request.form['income'])
            credit_score = float(request.form['credit_score'])
            debt = float(request.form['debt'])
            age = int(request.form['age'])  # Capture age as well

            # Validate inputs
            if income < 0 or credit_score < 0 or debt < 0 or age < 0:
                error = "Income, credit score, debt, and age must be non-negative."
            else:
                # Loading the trained model
                model = load_model()

                # Calculate debt-to-income ratio
                debt_to_income = debt / income

                # Prepare the input data as a DataFrame with the feature names
                input_data = pd.DataFrame([[income, credit_score, debt_to_income, age]],
                                          columns=['income', 'credit_score', 'debt_to_income', 'age'])

                # Make the prediction
                prediction = model.predict(input_data)

                # Display the result (Loan Approval or Denial)
                result = "Loan Approved!" if prediction[0] == 1 else "Loan Denied!"
        except ValueError:
            error = "Please enter valid numeric values for income, credit score, debt, and age."
        except Exception as e:
            error = f"An error occurred: {str(e)}"

    return render_template('index.html', result=result, error=error)

# Running the Application
if __name__ == '__main__':
    # Loading and preprocessing data, then train the model (only once)
    data = load_data()
    train_model(data)

    app.run(debug=True)
