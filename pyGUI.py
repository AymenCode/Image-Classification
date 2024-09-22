import PySimpleGUI as sg
import os.path

file_list_column = [
    [
        sg.Text("Image Folder"),
        sg.In(size=(25,1), enable_events=True, key="-FOLDER-"),
        sg.FolderBrowse(),
    ],
    [
        sg.Listbox(
            values=[], enable_events=True, size=(40,20),
            key="-FILE LIST-"
        )
    ],
]

image_viewer_column = [
    [sg.Text("Choose an image from the list on the left:")],
    [sg.Text(size=(100,1), key="-TOUT-")],
    [sg.Image(size=(800,600),key="-IMAGE-")],
]
list1=['Self Organizing Map (SOM)','Principal Component Analysis (PCA)']

Model_column = [
    [sg.Text("Choose The model you want to implement :")],
    [sg.Combo(list1, enable_events=True, readonly=True,default_value=list1[0], key='-METH-')],
]
layout = [
    [
        sg.Column(file_list_column),
        sg.VSeperator(),
        sg.Column(image_viewer_column),
        sg.VSeperator(),
        sg.Column(Model_column),
    ],
    
]

window = sg.Window("SOM/PCA tester", layout)
f = open("guess.txt", "r")
f1 = open("guess1.txt", "r")
nb=int(f.readline())
nb1=int(f1.readline())

L2=[f.readline().split(' ') for i in range(nb)]
L1=[f1.readline().split(' ') for i in range(nb)]
for i in range(nb) :
    L2[i][2] = L2[i][2][:-1]
    L2[i][0] = L2[i][0][:-4]

for i in range(nb) :
    L1[i][2] = L1[i][2][:-1]
    L1[i][0] = L1[i][0][:-4]

while True:
    event,values = window.read()
    
    if event == "Exit" or event == sg.WIN_CLOSED:
        break

    if event == "-FOLDER-":
        folder = values["-FOLDER-"]
        try:
            file_list = os.listdir(folder)
        except:
            file_list= []

        fnames = [
            f
            for f in file_list
            if os.path.isfile(os.path.join(folder,f))
            and f.lower().endswith((".png"))
        ]
        window ["-FILE LIST-"].update(fnames)
    elif event == "-FILE LIST-" or event == "-METH-":
        try:
            name =values["-FILE LIST-"][0][:-4]
            filename = os.path.join(
                values["-FOLDER-"], values["-FILE LIST-"][0]
            )

            if(values["-METH-"] == list1[0]):
                L=L2
                sen = "According to the SOM, "
            else:
                L=L1
                sen = "According to the PCA, "
            
            for k in range(nb) :
                if name == L[k][0]:
                    guess = L[k][1]
                    corr = L[k][0][0]
                    break
            sen += "I'm guessing it's " + L[k][2] + "% a "
            if guess == "1" :
                sen += "cat"
            elif guess == "2" :
                sen += "bear"
            else :
                sen += "lion"

            if corr == guess :
                sen += ". Which is right! :)"
            else :
                sen += ". Which is wrong :("
            window["-TOUT-"].update(sen)
            window["-IMAGE-"].update(size=(800,600), filename=filename)
        except:
            pass
window.close()
