# -*- coding: utf-8 -*-
"""
Created on Sat Apr 15 23:40:15 2023

@author: gagan
"""
# requirements: python3 + pytorch
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from itertools import cycle

import torchvision as tv
import torchvision.transforms as transforms

import matplotlib.pyplot as plt
import numpy as np
from sklearn.metrics import confusion_matrix
import seaborn as sns
from sklearn.preprocessing import LabelBinarizer
from sklearn.metrics import roc_auc_score
from sklearn.metrics import RocCurveDisplay

train_batch_size=64
test_batch_size=1000

three_cases = 0
class Net(nn.Module):
    def __init__(self):
        global three_cases
        super(Net,self).__init__()
        self.three_cases = three_cases
        self.conv1=nn.Conv2d(3,6,5,stride=1,padding=0)
        self.conv2=nn.Conv2d(6,16,5,stride=1,padding=0)
        self.fc1=nn.Linear(16*5*5,120)
        self.fc2=nn.Linear(120,84)
        self.fc3=nn.Linear(84,10)
        if(self.three_cases==1):
            self.apply(self._xavier_init)
        if(self.three_cases==2):
            self.apply(self._kaiming_normal_init)
    def _xavier_init(self, layer):
        if isinstance(layer, (nn.Conv2d, nn.Linear)):
            nn.init.xavier_uniform_(layer.weight)
            if layer.bias is not None:
                nn.init.constant_(layer.bias, 0.0)
    def _kaiming_normal_init(self, layer):
        if isinstance(layer, (nn.Conv2d, nn.Linear)):
            nn.init.kaiming_normal_(layer.weight)
            if layer.bias is not None:
                nn.init.constant_(layer.bias, 0.0)
    def forward(self,x):
        x=F.max_pool2d(F.relu(self.conv1(x)),2)
        x=F.max_pool2d(F.relu(self.conv2(x)),2)
        x=x.view(-1,self.num_flat_features(x))
        x=F.relu(self.fc1(x))
        x=F.relu(self.fc2(x))
        x=self.fc3(x)
        return x
    def num_flat_features(self,x):
        #x is a 4D tensor
        x_size=x.size()[1:]
        num=1
        for n in x_size:
            num*=n
        return num
def plot_cm(cm,title):
    plt.figure(figsize=(10,10))
    sns.heatmap(cm, annot=True, fmt='.4f')
    plt.xlabel('PREDICTED')
    plt.ylabel('ACTUAL')
    #plt.show()
    plt.savefig(title+".png", dpi=300)
def load_data(device):
    transform=transforms.Compose(
        [transforms.ToTensor(),
         transforms.Normalize((0.4914, 0.4822, 0.4465), (0.247, 0.243, 0.261))])
    train_set=tv.datasets.CIFAR10(
        root='./data',
        train=True,
        download=True,
        transform=transform
        )
    
    
    test_set=tv.datasets.CIFAR10(
        root='./data',
        train=False,
        download=True,
        transform=transform
        )
#    train_set, test_loader = train_set.to(device), test_set.to(device)
    train_loader=torch.utils.data.DataLoader(
        train_set,
        batch_size=train_batch_size,
        shuffle=True,
        num_workers=2)
    test_loader=torch.utils.data.DataLoader(
        test_set,
        batch_size=test_batch_size,
        shuffle=False,
        num_workers=2)
    print("data loaded successfully...")
    return train_loader,test_loader

def accuracy(model,x,device,neg=False):
    global a
    predictions = []
    labels_l = []
    op = []
    with torch.no_grad():
        correct=0
        total=0
        for data in x:
            
            images,labels=data
            images, labels = images.to(device), labels.to(device)
            a = images
            if neg:
            	images=-images
            outputs=model(images)
            _,predicted=torch.max(outputs.data,1)
            op.append(F.softmax(outputs,dim=1).data.cpu().numpy())
            total+=labels.size(0)
            correct+=(predicted==labels).sum().item()
            predictions += predicted.cpu().numpy().tolist()
            labels_l += labels.cpu().numpy().tolist()
        return (100 * correct / total),predictions, labels_l,op

def train(train_loader, test_loader, model, criterion, optimizer,epoch,device):
    model.train()
    running_loss=0
    for i,data in enumerate(train_loader,0):
        inputs,labels=data
        inputs, labels = inputs.to(device), labels.to(device)
        optimizer.zero_grad()
        outputs=net(inputs)
        loss=criterion(outputs,labels)
        loss.backward()
        optimizer.step()

        # print statistics
        running_loss+=loss.item()
        if i%200==199:
            print("[epoch %d, iter %5d] loss: %.3f"%(epoch+1,i+1,running_loss/200))
            running_loss=0.0
    train_acc,_,_,_=accuracy(model,train_loader,device)
    test_acc,_,_,_=accuracy(model,test_loader,device)
    print("epoch %d: train_acc %.3f, test_acc %.3f"%(epoch+1,train_acc,test_acc))
    return train_acc,test_acc

def display(train_acc,test_acc,title_,x_label,y_label):
    fig,ax=plt.subplots()
    ax.plot(range(1,len(train_acc)+1),train_acc,color='r',label='train_acc')
    ax.plot(range(1,len(test_acc)+1),test_acc,color='b',label='test_acc')
    ax.set_title(title_)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.legend(loc='lower right')
    #plt.show()
    plt.savefig(title_+".png", dpi=300)

def writedattofile(file,param,five_acc,max_acc,mean_acc,std,dataset):
    f = open(file, "a")
    f.write('---------------------------------------------\n')
    f.write(dataset+'\n')
    for key,value in param.items():
        f.write("{} = {}\n".format(key,str(value)))
    f.write("Accuracy in 5 runs = \n")
    for value in five_acc:
        f.write("{}\n".format(str(value)))
    f.write("Over 5 runs: Max accuracy = {} Mean Accuracy = {} STD = {}\n".format(max_acc,mean_acc,std))
    f.write('---------------------------------------------\n')
    f.close()
if __name__ == '__main__':
    class_n = ['airplane', 'automobile', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck']
    device = 'cuda' if torch.cuda.is_available() else 'cpu'
    train_loader,test_loader=load_data(device)
    test_loader2=torch.utils.data.DataLoader(tv.datasets.CIFAR10(root='./data',train=False,download=True),batch_size=test_batch_size,shuffle=False,num_workers=2)
    param = {}
    while three_cases<3:
        train_acc_list = []
        test_acc_list = []

        for i in range(5):
            
                
            # input MNIST
            
            
            # new model
            
            net=Net()
            net = net.to(device)
            
            # training
            max_epoch=20
            param["max_epoch"] = 20
            
            if(three_cases == 0):
                learning_rate=0.008
                param["Learning_Rate"] = 0.008
                momentum=0.9
                param["momentum"] = 0.9
                #weight_decay  = 0.0003
                #param["weight_decay"] = 0.0003
                
                criterion=nn.CrossEntropyLoss()
                criterion = criterion.to(device)
                optimizer=optim.SGD(net.parameters(),lr=learning_rate,momentum=momentum)
                #optimizer = optim.Adam(net.parameters(), lr=learning_rate)
                param["optimizer"] = "SGD"
                param["initialization"] = "Xavier"
            if(three_cases == 1):
                learning_rate=0.0003
                param["Learning_Rate"] = 0.0003
                #momentum=0.9
                weight_decay  = 0.0005
                param["weight_decay"] = 0.0005
                
                criterion=nn.CrossEntropyLoss()
                criterion = criterion.to(device)
                #optimizer=optim.SGD(net.parameters(),lr=learning_rate,momentum=momentum)
                optimizer = optim.Adam(net.parameters(), lr=learning_rate,weight_decay=weight_decay)
                param["optimizer"] = "Adam"
                param["initialization"] = "Xavier"
            if(three_cases == 2):
                learning_rate=0.0008
                param["Learning_Rate"] = 0.0008
                #momentum=0.9
                weight_decay  = 0.0001
                param["weight_decay"] = 0.0001
                criterion=nn.CrossEntropyLoss()
                
                criterion = criterion.to(device)
                #optimizer=optim.SGD(net.parameters(),lr=learning_rate,momentum=momentum)
                optimizer = optim.Adam(net.parameters(), lr=learning_rate,weight_decay=weight_decay)
                param["optimizer"] = "Adam"
                param["initialization"] = "Kaiming_normal"
                
        
            train_acc=[]
            test_acc=[]
            class_id = 0
            for epoch in range(max_epoch):
                train_acc_t,test_acc_t=train(train_loader, test_loader, net, criterion, optimizer,epoch,device)
                #train_loss, train_acc_t = train(train_loader, net, criterion, optimizer, epoch)
                #test_loss, test_acc_t = test(test_loader, net, criterion, epoch)
                train_acc.append(train_acc_t)
                test_acc.append(test_acc_t)
            train_acc_list.append(train_acc) 
            test_acc_list.append(test_acc) 
            if i==0:
                display(train_acc,test_acc,"CIFAR10 Classification with "+param["optimizer"]+" and "+str(param["Learning_Rate"])+" learning rate","Epochs","Accuracy")
                a,test_predictions,test_labels,y_score = accuracy(net,test_loader,device)
                
                y_score = np.array(y_score)
                y_score = y_score.reshape((y_score.shape[0]*y_score.shape[1], y_score.shape[2]))
                label_binarizer = LabelBinarizer().fit(train_loader.dataset.targets)
                y_onehot_test = label_binarizer.transform(test_labels)
                test_cm = confusion_matrix(test_labels, test_predictions)
                cm_norm = test_cm.astype('float') / test_cm.sum(axis=1)[:, np.newaxis]
                plot_cm(cm_norm,'Confusion Matrix between predicted and actual data')        
                flatten_conf_matrix = cm_norm.flatten()
                non_diagonal_indices = np.where(~np.eye(cm_norm.shape[0], dtype=bool)) 
                non_diagonal_values = cm_norm[non_diagonal_indices] 
                sorted_indices = np.argsort(non_diagonal_values)[::-1]
                top_3_indices = sorted_indices[:3] 
                class_of_interest = "0"
                for idx in top_3_indices:
                    row_idx = non_diagonal_indices[0][idx]
                    col_idx = non_diagonal_indices[1][idx]
                    wrong_idx = np.where((test_labels == row_idx) & (test_predictions == col_idx))[0][0]
                    x,y = test_loader2.dataset[wrong_idx]
                    x_wrong = x
                    #x_wrong = np.squeeze(x_wrong, axis=0)
                
                    #plt.imshow(x_wrong)
                    #plt.title("{} Test Label mislabed as {}".format(class_n[y],class_n[test_predictions[wrong_idx]]))
                    #plt.show()
                    fig, ax = plt.subplots()
                    ax.imshow(x_wrong)
                    ax.set_title("{} Test Label mislabed as {}".format(class_n[y],class_n[test_predictions[wrong_idx]]))

                    # Save the figure to a file
                    fig.savefig("{}to{}.png".format(y,test_predictions[wrong_idx]))
                    #plt.imsave("{}to{}.png".format(y,test_predictions[wrong_idx]),x_wrong)
                total_auc = 0
                n_classes = 10
                fig, ax = plt.subplots(figsize=(6, 6))
                colors = cycle(["aqua", "darkorange", "cornflowerblue", "purple", "green"])
                for class_id, color in zip(range(n_classes), colors):
                    RocCurveDisplay.from_predictions(
                        y_onehot_test[:, class_id],
                        y_score[:, class_id],
                        name=f"ROC curve for {class_n[class_id]}",
                        color=color,
                        ax=ax,
                    )
                    samples = sum(y_onehot_test[:,class_id])
                    total_auc += (samples/len(y_onehot_test[:,class_id]))*roc_auc_score(y_onehot_test[:, class_id],y_score[:, class_id])
                    param["total_auc"] = str(total_auc)
                plt.plot([0, 1], [0, 1], "k--", label="chance level (AUC = 0.5)")
                plt.axis("square")
                plt.xlabel("False Positive Rate")
                plt.ylabel("True Positive Rate")
                plt.title("One-vs-Rest ROC curves:\n")
                plt.legend()
                #plt.show()
                plt.savefig("One-vs-Rest ROC curves.png", dpi=300)

                    
    
                
                """with torch.no_grad():
                    x = x.unsqueeze(0).to(device)
                    outputs=net(x)
                    _,predicted=torch.max(outputs.data,1)"""
        test_np = np.array(test_acc_list)
        train_np = np.array(test_acc_list)
        
        writedattofile("test_CIFAR10.txt",param,list(test_np[:,-1]),np.amax(test_np, axis=0)[-1],np.mean(test_np, axis=0)[-1],np.std(test_np, axis=0)[-1],"CIFAR10")
        three_cases+=1

