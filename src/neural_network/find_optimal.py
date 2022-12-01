from subprocess import call
import csv
import datetime
import shutil

nb_hidden = 1
nb_neurons = 20
learning_rate = 0.05
threshold = [0.5, 0.15, 0.05, 0.005]
increment = [0.01, 0.005, 0.001, 0.0001]
success_percent = call(["./neural", str(nb_hidden), str(nb_neurons), str(learning_rate)])
current_best = [0, 0, 0, 0]


for nb_hidden in range(1, 5):
    with open('./saved_data/hyperparams_' + str(nb_hidden) + '.csv', mode='w') as csv_file:
        fieldnames = ['current_time', 'nb_hidden', 'nb_neurons', 'learning_rate', 'success_percent']
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)

        writer.writeheader()

        while (nb_neurons <= 80):
            while (learning_rate <= threshold[nb_hidden - 1]):
                success_percent = call(["./neural", str(nb_hidden), str(nb_neurons), str(learning_rate)])
                date_time = datetime.datetime.now().strftime("%d/%m/%Y, %H:%M:%S")
                writer.writerow({'current_time': date_time, 'nb_hidden': str(nb_hidden), 'nb_neurons': str(nb_neurons),
                                 'learning_rate': str(learning_rate), 'success_percent': str(success_percent)})
                if (success_percent > current_best[nb_hidden - 1]):
                    current_best[nb_hidden - 1] = success_percent
                    shutil.copy("./saved_data/weights.data", "./saved_data/best_weights_" + str(nb_hidden) + ".data")

                learning_rate += increment[nb_hidden - 1]
                learning_rate = round(learning_rate, 2)

            nb_neurons += 5
            learning_rate = threshold[nb_hidden - 1] / 10



