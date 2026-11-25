import matplotlib.pyplot as plt
import math as mt
dt=0.01                #intervalo de tempo/passo de integracao
g=-9.81                #aceleracao da gravidade em m/s^2
x=-10.00               #posicao inicial do projetil do estilingue no eixo horizontal
y=-2.00                #posicao inicial do projetil do estilingue no eixo vertical
x0=0.0                 #posicao de repouso do estilingue no eixo horizontal
y0=0.0                 #posicao de repouso do estilingue no eixo vertical
k=30.0                  #constante da mola
vx=0.0                 #componente horizontal da velocidade inicial 
vy=0.0                 #componente vertical da velocidade inicial 
t=0.0                  #tempo inicial
defx=x-x0              #deformacao inicial do estilingue no eixo horizontal
defy=y-y0              #deformacao inicial do estilingue no eixo vertical
# F=-kx                #Lei de Hooke
# F=ma                 #Segunda Lei de Newton
m=1.0                  #massa do projetil
dadosmolax=[]
dadosmolay=[]
print("Inicio simulação da mola")
while (x<0 or y<0):     #criterio para a forca da mola deixar de agir e restar apenas o movimento balistico. Possivel melhorar
	Fx=-k*defx       #componente horizontal da forca devido a mola
	Fy=-k*defy       #componente vertical da forca devido a mola
	ax=Fx/m          #aceleracao horizontal
	ay=Fy/m+g        #aceleracao vertical
	dvx=ax*dt        #diferencial horizontal da velocidade
	dvy=ay*dt        #diferencial vertical da velocidade
	vx=vx+dvx        #integracao da velocidade
	vy=vy+dvy        
	dx=vx*dt         #diferencial horizontal da posicao
	dy=vy*dt         #diferencial vertical da posicao
	x=x+dx
	y=y+dy
	defx=x-x0        #calculo da deformacao
	defy=y-y0
	dadosmolax.append(x)
	dadosmolay.append(y)
	print (x,y)
	
dadosx=[]
dadosy=[]
rhoCdA=0.08  #combinação das várias constantes em uma
#while (y>=0):
while (abs(vx>0.5)):
	t=t+dt
	rax=rhoCdA*vx*mt.sqrt(vx*vx)/(2*m)   # desaceleração devido à força de resistencia do ar, proporciona à secção reta transversal do objeto, densidade do fluido, coeficiente de arraste e do quadrado da velocidade
	ray=rhoCdA*vy*mt.sqrt(vy*vy)/(2*m)
	ay=g-ray              
	ax=-rax;
	dvy=ay*dt
	dvx=ax*dt
	vy=vy+dvy         
	vx=vx+dvx
	dx=vx*dt
	dy=vy*dt          
	x=x+dx
	y=y+dy
	dadosx.append(x)
	dadosy.append(y)
	if(y<0):
		y=0.00
		vy=-(vy)*0.8# perda de energia no impacto com o solo
		vx=vx*0.99 # perda tambem no eixo x, considerando que a massa não seja pontual		
	print (x,y)
plt.title("Lançamento de estilingue com resistência do ar")
plt.xlabel("Eixo horizontal")
plt.ylabel("Altitude")
plt.plot(dadosx,dadosy,"bo")
plt.plot(dadosmolax,dadosmolay,"ro")
plt.show()