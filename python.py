import serial
import pyautogui

ser = serial.Serial('COM3')  # change serial to your own port
data = [0, 0, 0, 0]
prevdata = [0, 0, 0, 0]
xsensitivity = 0.1
ysensitivity = 0.1
threshold = 175
xymove = [0, 0]
update = [False, False]
counter = [0, 0]
click = [False, False]


def leftClick():
    pyautogui.mouseDown(button='left')


def rightClick():
    pyautogui.mouseDown(button='right')


def leftRelease():
    pyautogui.mouseUp(button='left')


def rightRelease():
    pyautogui.mouseUp(button='right')


def move(x, y):
    pyautogui.moveRel(x, y)


def scroll(x):
    pyautogui.scroll(x)


def executeTask():
    if data[0] >= 4 and data[1] < 4:
        if click[0] == False:
            leftClick()
        click[0] = True
    else:
        if click[0] == True:
            leftRelease()
        click[0] = False
    if data[0] < 4 and data[1] >= 4:
        if click[1] == False:
            rightClick()
        click[1] = True
    else:
        if click[1] == True:
            rightRelease()
        click[1] = False
    if data[0] >= 4 and data[1] >= 4:
        if (abs(data[2] - prevdata[2]) > threshold) and xymove[0] == 0:
            print()
            #scroll((-data[2] + prevdata[2] + 25) * xsensitivity)
    if (abs(data[2] - prevdata[2]) > threshold) and xymove[0] == 0:
        move(0, (-data[2] + prevdata[2] + 25) * xsensitivity)
    if (abs(data[3] - prevdata[3]) > threshold) and xymove[1] == 0:
        move((data[3] - prevdata[3] + 25) * ysensitivity, 0)


def decode():
    string = str(ser.readline())[2:]
    print(string)
    try:
        if string[0] == '<':
            index = -1
            element = ""
            for character in string:
                if character == '<':
                    index += 1
                elif character == ' ' or character == ">":
                    data[index] = float(element)
                    element = ""
                    index += 1
                else:
                    element += character
                if character == ">":
                    break
    except:
        print()


def updatexy():
    if (update[0]):
        if (data[2] - prevdata[2] > threshold) and xymove[0] < 1:
            xymove[0] += 1
            counter[0] = 0
        if (data[2] - prevdata[2] < -threshold) and xymove[0] > -1:
            xymove[0] += -1
            counter[0] = 0
    if (update[1]):
        if (data[3] - prevdata[3] > threshold) and xymove[1] < 1:
            xymove[1] += 1
            counter[1] = 0
        if (data[3] - prevdata[3] < -threshold) and xymove[1] > -1:
            xymove[1] += -1
            counter[1] = 0


def updateMove():
    if (abs(data[2] - prevdata[2]) < threshold):
        update[0] = False
    else:
        update[0] = True
    if (abs(data[3] - prevdata[3]) < threshold):
        update[1] = False
    else:
        update[1] = True


def updateCounter():
    counter[0] += 1
    counter[1] += 1
    if (counter[0] == 3):
        xymove[0] = 0
        counter[0] = 0
    if (counter[1] == 3):
        xymove[1] = 0
        counter[1] = 0


while True:
    decode()
    updateCounter()
    updateMove()
    updatexy()
    print(prevdata)
    executeTask()
    prevdata = data.copy()
    print(data)
    print(update)
    print(xymove)
    print(click)
