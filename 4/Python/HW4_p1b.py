import numpy as np
from sklearn.metrics import accuracy_score
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import sys
from collections import Counter
filename = sys.argv[1]
data = np.genfromtxt(filename)
y = data[:, 0]

X = data[:, 1:] 
filename = sys.argv[2]
data = np.genfromtxt(filename)
y_test = data[:, 0]
X_test = data[:, 1:] 
classes = ['brick','grass','rice','stones'];

if 'pca' in filename:
    fig = plt.figure()
    ax = fig.add_subplot(121, projection='3d')
    ax2 = fig.add_subplot(122, projection='3d')

    for class_id in np.unique(y_test):
        class_mask = y_test == class_id
        cls = int(class_id)
        ax.scatter(X_test[class_mask, 0], X_test[class_mask, 1], X_test[class_mask, 2], label=f"Class {classes[cls-1]}")
    for class_id in np.unique(y):
        class_mask = y == class_id
        cls = int(class_id)
        ax2.scatter(X[class_mask, 0], X[class_mask, 1], X[class_mask, 2], label=f"Class {classes[cls-1]}")

    ax.set_xlabel('PC1')
    ax.set_ylabel('PC2')
    ax.set_zlabel('PC3')
    ax.set_title('Test Images 3-dimensional PCA ')

    ax2.set_xlabel('PC1')
    ax2.set_ylabel('PC2')
    ax2.set_zlabel('PC3')
    ax2.set_title('Train Images 3-dimensional PCA')
    ax.legend()
    ax2.legend()
    plt.show()
X_orig = X
X_test_orig = X_test
X = (X - np.mean(X, axis=0)) / np.std(X, axis=0)
X_test = (X_test - np.mean(X_orig, axis=0)) / np.std(X_orig, axis=0)
kmeans = KMeans(n_clusters=4,n_init = 500,tol = 1e-7, algorithm='lloyd')
kmeans.fit(X)
y_pred = kmeans.predict(X_test)
y_pred_n = kmeans.predict(X)
clusters = {}
total_dom_count = 0
entropy = 0
for i in range(4):
    indices = [j for j, x in enumerate(y_pred) if x == i]
    cluster_labels = y_test[indices]
    print(cluster_labels)
    if(len(cluster_labels)!=0):
        dominant_label = Counter(cluster_labels).most_common()[0][0]
        dom_count = Counter(cluster_labels).most_common()[0][1]
        prob = len(cluster_labels)/len(y_pred)
        per_entropy = - prob * np.log2(prob)
        entropy +=per_entropy

        total_dom_count +=dom_count
        clusters[i] = dominant_label
print(clusters)
correct = 0
for i in range(len(y_pred)):
    print("{} is class and {} is pred_clus".format(y_test[i],y_pred[i]))
    if(y_pred[i] in clusters):
        if y_test[i] == clusters[y_pred[i]]:
            correct += 1

accuracy = correct / len(y_pred)

print("Kmeans entropy:",entropy )
print("Kmeans Accuracy:", accuracy)


from sklearn import svm

clf = svm.SVC(kernel='rbf')
clf.fit(X, y)
y_pred = clf.predict(X_test)
from sklearn import metrics
print("SVM Accuracy:",metrics.accuracy_score(y_test, y_pred))
