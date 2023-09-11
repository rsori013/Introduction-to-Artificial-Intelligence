# Abhinav Allam (aalla009) 862200322 
# Jacqueline Gardea (jgard046) 862305608 
# Majd Kawak (mkawa025) 862273310 
# Rovin Soriano (rsori013) 862149089

import random
import math
import numpy as np
import pandas as pd
from collections import Counter
import time
from sklearn.tree import DecisionTreeClassifier


def leave_one_out_cross_validation(data_passed, current_set, feature_to_add_or_remove, flag):
    # Add labels
    data = [data_passed[0]]
    # Add current all features except if flag is 1, then don't add feature_to_remove
    for feature in current_set:
        if flag == 1:
            if feature != feature_to_add_or_remove:
                data.append(data_passed[feature])
        else:
            data.append(data_passed[feature])
    # Add left over feature from flag 0  we're processing
    if flag == 0:
        data.append(data_passed[feature_to_add_or_remove])
    # Number of right classification
    number_correctly_classified = 0
    # Iterate through columns features
    for object_to_classify, object_class_label in enumerate(data[0]):
        # Object Index      Object Label 1 or 2

        # Calculates nearest neighbor distance
        nearest_neighbor_distance = float('inf')
        nearest_neighbor_location = None
        # Iterating through all features columns except first label column
        for k in range(len(data[0])):
            if k != object_to_classify:
                distance = 0
                # print("Ask if ", object_to_classify, " is nearest neigbour with ", k)
                for feature_column in data[1:]:
                    distance += math.pow(feature_column[object_to_classify] - feature_column[k], 2)
                distance = math.sqrt(distance)
                if distance < nearest_neighbor_distance:
                    nearest_neighbor_distance = distance
                    nearest_neighbor_location = k
        # Check whether it classified it right
        if object_class_label == data[0][nearest_neighbor_location]:
            number_correctly_classified += 1
        # print("Object ", object_to_classify, " is in class ", object_class_label)
        # print("Its nearest_neighbor is ", nearest_neighbor_location, " which is in class ", data[0][nearest_neighbor_location])

    # Calculate Accuracy
    accuracy = number_correctly_classified / len(data[0])
    # Return Accuracy result
    return accuracy * 100


def forward_feature_selection(data):
    # Start measuring the execution time
    start_time = time.time()
    # Count the frequency of each number
    frequency_counter = Counter(data[0])
    # Find the number with the highest frequency
    most_common = frequency_counter.most_common(1)
    # Extract the number and its frequency
    highest_frequency_number = most_common[0][0]
    highest_frequency = most_common[0][1]
    print("\nRunning nearest neighbor with no features (default rate), using “leaving-one-out” evaluation, "
          "we get an accuracy of ", (highest_frequency / len(data[0])) * 100, "%")
    # Holds our total features set
    total_features_set = []
    # Save best total
    total_best_feature_set = []
    total_best_accuracy = -1
    print("---------------------------------------------")
    print("Beginning search (Forward Feature Selection).")
    print("---------------------------------------------")
    # Iterate through columns features except the first
    for col_index, column in enumerate(data[1:], start=1):
        print("On level ", col_index, " of the search tree\n")
        # Holds our current features at each level
        features_for_Level = -1
        best_so_far_accuracy = 0
        # Access columns features except the first
        for feature_index, value in enumerate(data[1:], start=1):
            # If a feature is not in our total set, consider it
            if feature_index not in total_features_set:
                # Accuracy function with flag 0 to add
                accuracy = leave_one_out_cross_validation(data, total_features_set, feature_index, 0)
                print("--Considering adding feature number ", feature_index, " gets us a total accuracy of ", accuracy,
                      "%")
                if accuracy > best_so_far_accuracy:
                    best_so_far_accuracy = accuracy
                    features_for_Level = feature_index
        total_features_set.append(features_for_Level)
        if total_best_accuracy < best_so_far_accuracy:
            total_best_feature_set.append(features_for_Level)
            total_best_accuracy = best_so_far_accuracy
        else:
            print("\n**Warning, Accuracy has decreased! Continuing search in case of local maxima**")
        print("\nOn level ", col_index, " feature {", features_for_Level, "} was added to current set")
        print("Current Set of Features: ", total_features_set, " with accuracy of ", best_so_far_accuracy, "%")
        print("---------------------------------------------")

    # Output best accuracy
    print("Set ", total_best_feature_set, " has the highest accuracy:", total_best_accuracy)
    # End measuring the execution time
    end_time = time.time()
    elapsed_time = end_time - start_time
    rounded_time = round(elapsed_time, 3)
    print("Elapsed time:", rounded_time, "seconds")


def backward_feature_elimination(data):
    # Start measuring the execution time
    start_time = time.time()
    # Holds our total features set
    total_features_set = [i for i in range(1, len(data))]
    accuracy_with_all_features = leave_one_out_cross_validation(data, total_features_set, 0, 1)
    print("\nRunning nearest neighbor with all the features, using “leaving-one-out” evaluation, "
          "we get an accuracy of ", accuracy_with_all_features, "%")
    # Save best total
    total_best_feature_set = total_features_set.copy()
    total_best_accuracy = accuracy_with_all_features
    print("---------------------------------------------")
    print("Beginning search (Backward Feature Elimination).")
    print("---------------------------------------------")
    # Iterate through columns features except the first -- inverse
    for col_index in reversed(range(1, len(data))):
        print("On level ", col_index, " of the search tree\n")
        # Holds our current features at each level
        features_for_Level = -1
        best_so_far_accuracy = 0
        # Access columns features except the first -- in reverse
        for feature_index in reversed(range(1, len(data))):
            # If a feature is in our total set, consider it
            if feature_index in total_features_set:
                # Accuracy function with flag 1 to remove
                accuracy = leave_one_out_cross_validation(data, total_features_set, feature_index, 1)
                print("--Considering removing feature number ", feature_index, " gets us a total accuracy of ",
                      accuracy, "%")
                if accuracy > best_so_far_accuracy:
                    best_so_far_accuracy = accuracy
                    features_for_Level = feature_index

        total_features_set.remove(features_for_Level)

        if total_best_accuracy < best_so_far_accuracy:
            total_best_feature_set = total_features_set.copy()
            total_best_accuracy = best_so_far_accuracy
        else:
            print("\n**Warning, Accuracy has decreased! Continuing search in case of local maxima**")
        print("\nOn level ", col_index, " feature {", features_for_Level, "} was removed from current set")
        print("Current Set of Features: ", total_features_set, " with accuracy of ", best_so_far_accuracy, "%")
        print("---------------------------------------------")

    # Output best accuracy
    print("Set ", total_best_feature_set, " has the highest accuracy:", total_best_accuracy)
    # End measuring the execution time
    end_time = time.time()
    elapsed_time = end_time - start_time
    rounded_time = round(elapsed_time, 3)
    print("Elapsed time:", rounded_time, "seconds")


def random_feature_combinations(data, num_features, num_subsets, num_features_in_each_set):
    # Start measuring the execution time
    start_time = time.time()
    print("\nNum of total features: ", num_features, " Features number in one set: ", num_features_in_each_set)
    # Generate unique random pairs of feature columns
    features_sets = []
    # Holds unique sets of features combinations
    unique_combinations = set()
    # Generate feature sets
    while len(unique_combinations) < num_subsets:
        # Generate a random pair of feature columns
        feature_indices = np.random.choice(range(1, num_features + 1), size=num_features_in_each_set, replace=False)
        feature_pair = tuple(sorted(feature_indices))
        # Check if the generated pair is unique
        if feature_pair not in unique_combinations:
            unique_combinations.add(feature_pair)
            features_sets.append(feature_pair)

    print("\nFeatures sets: ", features_sets)
    # Initialize an empty list to store accuracies for each feature pair
    accuracies = []
    print("---------------------------------------------")
    print("Beginning search (Random Feature Combinations).")
    print("---------------------------------------------")
    # Perform leave-one-out cross-validation for each feature pair
    for feature_pair in features_sets:
        # Perform leave-one-out cross-validation and get the accuracy
        accuracy = leave_one_out_cross_validation(data, feature_pair, 0, 1)
        # Save the accuracy in the accuracies list
        accuracies.append(accuracy)
        # Print the accuracy for the current feature pair
        print("Accuracy for feature set", feature_pair, ":", accuracy)

    # Find the index of the pair with the highest accuracy
    max_accuracy_index = np.argmax(accuracies)

    # Print the pair with the highest accuracy
    highest_accuracy_set = features_sets[max_accuracy_index]
    highest_accuracy = accuracies[max_accuracy_index]
    print("\nSet ", highest_accuracy_set, " has the highest accuracy:", highest_accuracy)

    # End measuring the execution time
    end_time = time.time()
    elapsed_time = end_time - start_time
    rounded_time = round(elapsed_time, 3)
    print("Elapsed time:", rounded_time, "seconds")


# Z Normalization
def z_normalization(arr):
    mean_val = np.mean(arr)
    std_val = np.std(arr)
    normalized_arr = (arr - mean_val) / std_val
    return normalized_arr


print("Welcome to Abhinav's, Majd's, Jacqueline's and Rovin's Feature Selection Algorithms.")
# Validate if a file does exists
valid_file = False
while not valid_file:
    # Get the file path
    file_name = input("Type in the name of the file to test: ")
    try:
        # Read the entire file as a single string
        with open(file_name, "r") as file:
            content = file.read()
        # Split the content by newlines to get rows
        rows = content.split("\n")
        # Initialize a list to store the column values for each row
        rows_columns = []
        for index, row in enumerate(rows):
            # Split each row by tabs to get columns
            row_values = row.split()
            if index == 0:
                for index, row_value in enumerate(row_values):
                    # Append or extend value
                    col = [float(row_value)]
                    rows_columns.append(col)
            else:
                for index, row_value in enumerate(row_values):
                    rows_columns[index].append(float(row_value))
        # Close file
        file.close()
        print("File processed successfully.")
        valid_file = True
    except FileNotFoundError:
        print("File not found. Please try again.")

print("Type the number of the algorithm you want to run.")
print("1- Forward Selection.")
print("2- Backward Elimination.")
print("3- Random Combinations.")
algorithm_number = int(input())

print("\nThis dataset has", len(rows_columns) - 1, "features (not including the class attribute), with",
      len(rows_columns[0]), "instances.")
print("Please wait while I normalize the data...")

normalize_data = input("\nDo you want to Z Normalize the data values? (y/n): ")
if normalize_data.lower() == "y":
    # Normalize each column except the first
    for i in range(1, len(rows_columns)):
        column = rows_columns[i]
        normalized_column = z_normalization(column)
        rows_columns[i] = normalized_column
    print("Done!")

if algorithm_number == 1:
    forward_feature_selection(rows_columns)
elif algorithm_number == 2:
    backward_feature_elimination(rows_columns)
elif algorithm_number == 3:
    # Number of features
    default_num_subsets = len(rows_columns) - 1
    # Calculate the logarithm of the number of features
    default_num_features_in_each_set = math.ceil(math.log(default_num_subsets))
    # Ask the user if they want to change the default values
    print("\nDefault number of subsets is ", default_num_subsets, "and number of features in each set is log(",
          default_num_subsets, ")=", default_num_features_in_each_set)
    change_defaults = input("Do you want to change the default values? (y/n): ")
    if change_defaults.lower() == "y":
        default_num_subsets = int(input("Enter the desired number of subsets: "))
        default_num_features_in_each_set = int(input("Enter the desired number of features in each subset: "))
    random_feature_combinations(rows_columns, len(rows_columns) - 1, default_num_subsets, default_num_features_in_each_set)
else:
    print("Invalid algorithm number. Please choose a valid option.")
