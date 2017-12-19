from serial import Serial
import time

class Robo(object):
    def __init__(self, identificacao, a, b):
        self._posX = a
        self._posY = b
        self._id = identificacao
        self._direction = "RIGHT"
        self._done = False
        self._sleeping = False

    def getID(self):
        return self._id

    def getPosX(self):
        return self._posX

    def setPosX(self, x):
        self._posX = x
        if self.getPosX() == self.getTargetX() and self.getPosY() == self.getTargetY():
            self.setDone()
    
    def getPosY(self):
        return self._posY
    
    def setPosY(self, y):
        self._posY = y
        if self.getPosX() == self.getTargetX() and self.getPosY() == self.getTargetY():
            self.setDone()

    def getDirection(self):
        return self._direction

    def setDirection(self, newDirection):
        self._direction = newDirection

    def setTarget(self, X, Y):
        self.targetX=X
        self.targetY=Y
        
    def getTargetX(self):
        return self.targetX
            
    def getTargetY(self):
        return self.targetY

    def setDone(self):
        self._done = True

    def done(self):
        return self._done

    def sleep(self):
        self._sleeping = True

    def isSleeping(self):
        return self._sleeping

    def wakeUp(self):
        self._sleeping = False

class Controle(object):
    def __init__(self, robo1, robo2):
        self.r0 = robo1
        self.r1 = robo2
        self.ser = Serial('/dev/ttyUSB0', 9600)
        self.start()

    def sendMessage(self, robo, mensagem):
        mensagem = str(robo.getID()) + mensagem
        self.ser.write(mensagem.encode())

    def receiveMessage(self):
        mensagem = self.ser.read()
        return mensagem

    def start(self):
        self.sendMessage(self.r0, "S")
        self.sendMessage(self.r1, "S")
        self.run()

    def run(self):
        while not self.r0.done() or not self.r1.done():
            id = self.receiveMessage()
            
            if id == "0" and not self.r0.done():
                self.calculateRoute(self.r0, self.r1)
                
            elif id == "1" and not self.r1.done():
                self.calculateRoute(self.r1, self.r0)
            
        
    def calculateRoute(self, movingRobot, stationaryRobot):
        newPosition = self.getNextPosition(movingRobot)                
##        if newPosition[0] == stationaryRobot.getPosX() and newPosition[1] == stationaryRobot.getPosY():
##            self.resolveConflict1(movingRobot)
##        else:
##            self.updateRobot(movingRobot, newPosition[0], newPosition[1], newPosition[2])
##            if stationaryRobot.isSleeping():
##                self.calculateRoute(stationaryRobot, movingRobot)
            
        if newPosition[0] == stationaryRobot.getPosX() and newPosition[1] == stationaryRobot.getPosY():
            self.resolveConflict2(stationaryRobot)
        self.updateRobot(movingRobot, newPosition[0], newPosition[1], newPosition[2])
            
        
            
    def getNextPosition(self, robo):
        newX, newY, newDirection = robo.getPosX(), robo.getPosY(), robo.getDirection()
        if robo.getPosY() < robo.getTargetY():
            newY += 1
            newDirection = "RIGHT"
        elif robo.getPosY() > robo.getTargetY():
            newY -= 1
            newDirection = "LEFT"
        elif robo.getPosX() < robo.getTargetX():
            newX += 1
            newDirection = "DOWN"
        elif robo.getPosX() > robo.getTargetX():
            newX -= 1
            newDirection = "UP"            
        return [newX, newY, newDirection]

    """
    Sobre "getInstruction":
    O robo recebe como parametro a direcao que ele deve andar. Como o nosso
    robo nao consegue se orientar dentro do cenario com "UP, DOWN, LEFT, RIGHT",
    pois ele so consegue entender as suas proprias orientacoes "Front, Left, Right",
    utilizamos entao a funcao GetDirection para comparar a orientacao do robo
    com a proxima posicao do mapa, enviando entao para o robo as instrucoes F, L ou R.
    
    """
    
    def getInstruction(self, robo, new_direction):
        instruction = ""
        if robo.getDirection() == "RIGHT":
            if new_direction == "RIGHT":
                instruction = "F"
            elif new_direction == "UP":
                instruction = "L"
            elif new_direction == "DOWN":
                instruction = "R"
        elif robo.getDirection() == "LEFT":
            if new_direction == "LEFT":
                instruction = "F"
            elif new_direction == "DOWN":
                instruction = "L"
            elif new_direction == "UP":
                instruction = "R"
        elif robo.getDirection() == "DOWN":
            if new_direction == "DOWN":
                instruction = "F"
            elif new_direction == "RIGHT":
                instruction = "L"
            elif new_direction == "LEFT":
                instruction = "R"
        elif robo.getDirection() == "UP":
            if new_direction == "UP":
                instruction = "F"
            elif new_direction == "LEFT":
                instruction = "L"
            elif new_direction == "RIGHT":
                instruction = "R"
        robo.setDirection(new_direction)
        return instruction

    def updateRobot(self, robo, newPosX, newPosY, newDirection):
        robo.wakeUp()
        robo.setPosX(newPosX)
        robo.setPosY(newPosY)
        print("Robo",str(robo.getID()),":", robo.getPosX(), robo.getPosY(), newDirection)
        instruction = self.getInstruction(robo, newDirection)
        self.sendMessage(robo, instruction)
    
    def resolveConflict1(self, robo):
        robo.sleep()

    def resolveConflict2(self, robo):
        newX, newY, newDirection = robo.getPosX(), robo.getPosY(), robo.getDirection()
        if robo.getPosX()>robo.getTargetX() and robo.getPosX() > 0:
            newX += 1
            newDirection = "DOWN"
            time.sleep(5)
        else:
            newX -= 1
            newDirection = "UP"
            time.sleep(5)
        self.updateRobot(robo, newX, newY, newDirection)
	


"""
INA Robot:

Instrucoes:
1 - Iniciar objetos "Robo" com ID, Posicao Inicial Y, Posicao Inicial X
2 - Setar destino de cada robo com posicao Y e X "setTarget(y,x)"
3 - Executar "Controle" com cada robo como parametro

"""

"""
#teste 1
r1 = Robo(0, 1,0)
r2 = Robo(1, 1, 0)
r1.setTarget(4, 1)
r2.setTarget(2, 4)
##r2._direction = "RIGHT"
"""
"""
#teste 2
r1 = Robo(0, 2,0)
r2 = Robo(1, 1, 0)
r1.setTarget(1, 4)
r2.setTarget(1, 4)
"""

#teste 3
r1 = Robo(0, 4, 4)
r2 = Robo(1, 1, 0)
r1.setTarget(0, 1)
r2.setTarget(1, 4)
r1._direction = "LEFT"


Controle(r1, r2)
