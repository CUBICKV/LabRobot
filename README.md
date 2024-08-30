LabRobot
====

El robot nombrado “MATE” es un vehículo autónomo diseñado en un software de CAD, y con circuitos en simuladores electrónicos, programado en la plataforma ARDUINO.
Para el sistema de control, se utilizó una placa de expansión de Arduino nano la cual nos permitió usar sus pines junto a los cables jumpers, para ahorrar nodos de soldadura en el cableado.
Los niveles lógicos a trabajar para el control, se establecieron a 5v, el voltaje de funcionamiento del Arduino nano. Los modulos seleccionados fueron los que tuvieron ese mismo nivel.
Los componentes junto con sus funciones son:
Giroscopio MPU6050: Es un módulo que me permite saber la orientación y ángulo de giro en el Eje Z, para poder conocer mi sentido de giro y mi localización angular en el plano.
Sensor Touch de inducción: Es un sensor táctil, que realiza la misma función de un botón, pero le da una mayor estética al encendido, y nos ahorra el cableado del botón, al igual que es más grande que un botón convencional y menos estorboso
Micro Switch: Nos envía la energía de la Power bank para poder alimentar el sistema de control electrónico.
Puente H: Este es la interfaz de potencia que tiene el cerebro con el motor DC para la tracción trasera del vehículo.
Husky Lens: Es la cámara de visión artificial, conectada con un protocolo de comunicación al cerebro, procesa los datos de los objetos y los envía al control.
Sensor ultrasónico: Se colocaron 3 sensores ultrasónicos en la parte frontal, y en los dos laterales delanteros, para poder crear una referencia de donde se encuentran las paredes.
Arduino nano: Es el cerebro principal, se ocupan todos los pines digitales para poder controlar y recibir los datos de los sensores y componentes electrónicos. Tambien se conecta la cámara. Los protocolos utilizados son los siguientes:
-I2C: Para la cámara de visión artificial.
-PWM: Se ocupa para controlar el servo-motor y el puente h.
-Pulsos: Los recibe y envía para el puente h, el sensor capacitivo y los sensores ultrasónicos.
Indicador de batería: Se coloco un indicador de batería para conocer el voltaje actual de las celdas de litio.
Para la gestión de energía, es regla indispensable siempre conseguir una fuente para potencia y una independiente para control; tambien es preciso no combinar voltajes entre sí, es por eso que, para ahorrar un circuito de regulación, colocamos una tercera fuente de voltaje para el servo-motor.
Esta separación previene el ruido eléctrico y los sobre picos de voltaje que se puedan generar y alteren el procesador del Arduino.
Las fuentes se compraron calculando el consumo que podría tener cada sistema por separado, los cálculos estan registrados en la bitacora, y los resultados son estos:
Control:
-Arduino nano: 64mA
-MPU-6050: 45mA
-Sensor inductivo: 5mA
-Puente H: 36mA
-Husky Lens: 230mA
-3 Sensores Ultrasónicos: (40mA)3x 
Se estableció una Power bank exclusivamente para este sistema, de esa forma queda aislado de cualquier rebote, y nos brinda la energía suficiente para no tener fallas de corriente.
Servo-motor:
-Servo-motor: 1A
La fuente se estableció como una Power bank de 5v, en este caso, obtenemos una fuente que alimenta a un solo componente fuerte y evita el ruido
Motor DC:
-Motor 0.6A
-Indicador batería 0.1A
La fuente se estableció a base de 4 celdas 18650 en serie, para sumar voltajes, de esa forma obtenemos una potencia mayor a la de nuestra carga. 
Despues del cálculo de las fuentes, conseguimos los cargadores necesarios para poder recargas nuestras baterías y obtuvimos un cálculo con el consumo y la carga, que nos indicó un maximo de 4 horas de autonomía con lapsos de 2.6 horas de carga entre cada lapso.
Todas las fuentes se encuentran con un indicador de carga para saber en qué momento requieren ser recargadas; las Power bank se encuentran unidas por la tierra, para poder controlar el servo, pero las cedas de litio se encuentran aisladas y solo se permite el paso de corriente al motor a través del puente H.
Una vez conociendo la distribución de los elementos y los modulos electrónicos, el diseño final lo logramos pasando a digital todos los componentes, utilizando herramientas como vernier y Micrometro, para tener una precisión en las piezas.
En el software Solid Works recreamos los elementos y empezamos el diseño de los mecanismos y la estructura. Se diseño con 3 pisos para poder tener un control y ruteado de cables lo más estético y eficiente posible.
El primer piso se establecido solo para las partes mecánicas de dirección y tracción, al igual que los sensores ultrasónicos para poder crear un sistema de radar de posición de las paredes.
Se coloco el soporte para el motor, servo-motor y la base para las baterías Power bank. Y 4 soportes para mantener el piso 2.
En el segundo piso, se colocó en la parte delantera el soporte para la cámara, el hueco para las Power bank y en la parte trasera, las celdas de litio, junto con el conector de las baterías; igualmente se colocó 6 pilares para sostener el tercer piso.
Con el fin de que este lo más aislado del ruido que pueda generar los motores, se colocó la electronica en el tercer piso, teniendo el módulo de potencia (puente H, indicador de baterías y switch) en la parte trasera, el hueco de las Power bank en el centro, y en la parte frontal, el Arduino con el shield de expansión, el sensor Touch y el módulo MPU-6050.
Todo el sistema mecánico y electrónico y la estructura, estan fijos en su mayoría con tornillería de Acero al Carbon, y en algunas partes con silicón, el cableado soldado está cubierto con cinta doble cara y todo esta ruteado alrededor de los huecos y soportes que se dejaron en la estructura con cintos.
La estructura fue impresa en su totalidad en colores blanco y negro, dejando las BASES en blanco y los soportes y mecanismos color negro para dar contraste. El material utilizado fue el PET, ya que la impresora con que contamos es de materiales limitados y el más fuerte y resistente era el PET, por lo que nuestras piezas cuentan con una densidad del 40%, dándonos el nivel necesario entre peso y dureza.
La normativa indicaba que debían ser visibles todos los componentes, mecanismos y cableados, por eso se eligió un diseño estructural sencillo de varios pisos. Las dimensiones del auto fueron medidas en el Software Solid Works para cumplir con las normas de dimensiones y maso del vehículo, quedando como:
Medida X: 285mm
Medida Y: 195 mm
Medida Z: 200 mm
Peso: 1378g
El modelo resultante consta de cuatro ruedas de tipo neumático con un perímetro de 10cm, de las cuales dos son traseras, y se encuentran unidas por una varilla de transmisión, esta varilla se mueve en conjunto con las ruedas, gracias a un par de engranajes, de 30 y 17 dientes para la varilla y el engranaje del motor, respectivamente, el engranaje del motor fue impreso y moldeado a la forma de eje del motor con una pistola de calor. 
Nuestro motor es un motorreductor modelo TS-37GB330 de la marca AZSSMUK, sus especificaciones técnicas son las siguientes: 
Voltaje nominal: 12v
Revoluciones por minuto: 45-5400 rpm
Radio del eje: 37mm
Torque: 0.5kg/cm
Reducción: 1/9
Despues de considerar diferentes modelos, que fueron desde uno para Go-Car o uno de bomba de diafragma, se decidió cambiar fuerza por velocidad para poder mover el auto con un bajo consumo energético.
El motor está controlado por un módulo de puente H “L298n”, el cual nos permite controlar el sentido de giro y la velocidad, se conectó en el lateral derecho, precisamente para que el cableado estuviera lo más cercano entre el puente, la batería y el motor. Igualmente, si manteníamos el peso cercano al motor, reduciríamos la necesidad de potencia para poder romper el momento de inercia del vehículo.
La dirección del vehículo está colocada en la parte frontal con un servo-motor de 15kg, (entre mayor sea la capacidad del motor, menos energía requiere por un mayor esfuerzo), este está unido a un engranaje de 22 dientes que hace la función de un piñón, por medio de tornillos, de forma que cuando el servo gira entre 0° y 180°, el piñón en conjunto con una cremallera, convierten el movimiento circular en lineal. Con una relación de 1mm por cada 5° de giro.
Los ejes de las llantas son independientes, y estan sobre un eje rotativo sujeto a la placa de soporte “BASE_1” este se encarga de girar el eje de las llantas, y sostener el vehículo sobre su tapa.
Los ejes rotativos se encuentran unidos en 3 zonas por ejes, para darle movimientos consistentes e iguales a las 2 llantas, se encuentra uno en la parte inferior para mantener nivelados ambos ejes; en la parte superior a la altura del eje, para darles un movimiento proporcional y estabilidad entre sí; y finalmente, se encuentra la cremallera del motor en la parte trasera del eje, para transmitir el movimiento del servo-motor.
El servo motor se encuentra con una inclinación para aplicar presión del piñón sobre la cremallera, de esa forma, evitamos que se corra la barra por soltura, y concentramos la fuerza correctamente para la transmisión circular-lineal.
La distribución de la dirección se encuentra en la parte frontal, para tener una estabilidad y mejor control de la dirección, asi como una compensación para el peso de baterías y componentes electrónicos en el control.
Como todo buen proyecto, se debieron hacer cálculos cinemáticos para evaluar las velocidades del vehículo, estan registradas en la bitacora, y como resultado nos dio:
Velocidad lineal de 3km/h.
Una vez probado el vehículo y haciendo estimaciones que tenemos en la bitacora, se obtuvo que, al aplicar la velocidad estimada al peso del vehículo, junto al sistema reductor de engranajes en la transmisión, obtuvimos un resultado un tanto esperado:
El vehículo se desplaza de 0.667 a 0.706 km/h, con mayor tiempo, esto puede indicar que existe una aceleración que incrementa con el tiempo, al romper el momento de inercia de la masa del vehículo, le resulta más facil al motor moverse y eso implica que su velocidad crucero debe estar entre los 10 o 15 segundos, sin embargo, al tener que dar constantemente vueltas, podemos decir que su velocidad estándar estará entre 0.65 y 0.7 km/h.
El torque del vehículo depende de los componentes electromecánicos, en este caso de los mecanismos de tracción y dirección.
Para tracción, podemos iniciar con el torque del motor que este sujeto a un mecanismo de engranajes que cambia distancia por fuerza, es decir que aumenta nuestro torque en una relación de 0.55 a 1, es decir que incrementa en un 55% la fuerza de torque del motor, dando un resultado de: 9kg/cm^2
En cuanto a la dirección delantera, el mecanismo piñón cremallera, tiene una conversión circular a lineal con la relación es 5:1, pero en cuanto a fuerza, esto se mantiene igual, es decir una relación 1:1 en cuanto a torque, por lo que la fuerza de giro es igual a la del reductor de servo, es decir, una relación de 15Kg/cm^2. Esto solo es dinámico, pero la realidad es que los esfuerzos dependen de la resistencia de las piezas y sus propiedades (Tabla en la bitacora).
Todos los componentes estan calculados con su maximo esfuerzo energético, y su maximo peso, al igual que su minimo rendimiento de fuerza o velocidad, esto para poder tener una tolerancia de funcionamiento ante eventualidades.
El codigo está seccionado para una mejor interpretación, en la primera parte, estan incluidas las librerias, las cuales controlan la cámara, el monitor serial, el protocolo I2C, el giroscopio, y el servomotor.
Despues declaramos los pines del Arduino, desde el 3 hasta el 12, y creamos los objetos que serán controlados tanto para el protocolo I2C como el servo motor para PWM.
Declaramos las variables de control para el puente H.
Declaramos las variables de control para el giroscopio.
Declaramos los ángulos para el servo en cada posición.
Declaramos las variables para los 3 ultrasónicos.
Declaramos las condiciones para el sensor táctil.
Ponemos variables y colores para la cámara.
Definimos valores para la dirección.
Iniciamos el “Void setup”, en el cual inicializamos la comunicación serial a una velocidad de 115200, y tambien el protocolo Wire, para el mpu y su dirección, damos un mensaje de inicialización para confirmar que se inició todo, y entramos a un ciclo para iniciar la cámara, cuando este termine, definimos la función de los pines, e iniciamos todo para que comience apagado y en posición recta. Esperamos 2 segundos por algun fallo y da un mensaje de “LISTO” para comprobar que todo este correcto.
Iniciamos el “Void Loop”, Iniciamos la condición de inicio en la parte de arranque con el sensor Touch, este es un siclo anidado en donde se detecta el nivel del botón y en cuanto a eso, nos indica para pasar al siguiente bloque que sería el del sentido.
En este bloque, el robot está programado para avanzar recto hasta detectas 1 metro de distancia frente a él, en ese punto, va a detenerse, medir distancias y determinar si está en sentido de las manecillas del reloj o en sentido contrario. Una vez que determine eso, lo guardara en una variable (PARED) y esta determinara las direcciones de giro posteriores. El robot girara en el sentido indicado y comenzara el siclo de sensores.
En el ciclo de sensores, está el bloque de verificación, donde se muestran en el monitor serial todos los datos de los sensores y la cámara, así como los necesarios para el control del robot y detección de objetos.
Pasamos al bloque de giroscopio, donde se realiza una lectura del sensor, y usamos el ángulo Z para definir la orientación del vehículo, en este caso entra a condicionales que determinaran la zona donde se encuentra y el ángulo que lleva en ese momento, así como un conteo de la vuelta en donde se encuentra.
Pasando al bloque de Ultrasónico, tenemos el control de los 3 ultrasónicos, cada uno despues del otro, con 1 milisegundo de diferencia, usando la formación de escritura para centímetros.
Despues de eso, pasamos al apartado de la HUSKYLENS, donde verificamos que el protocolo sea correcto y que se encuentren objetos en pantalla, si todo eso es correcto, entonces llamaremos a la función HUSKYLENSResult, que se encuentra despues de concluir el Void Loop.
La función HUSKYLENSResult, detecta el bloque donde se encuentra el color, y al clasificarlo con un switch y ciclos if, nos guarda las cordenadas del bloque VERDE y el bloque ROJO, por medio del ID de la cámara (Para guardar los objetos en la cámara, es necesario seguir las instrucciones de la página de la Husky Lens para objetos múltiples).
Siguiendo a la Zona de control donde se procesarán todos los datos de los sensores, se coloca una secuencia de inicio, para poder detectar el sentido de giro del vehículo, usando los 3 sensores ultrasónicos, acto seguido, se guardan los datos y se ingresa al ciclo de control.
Este consta de un PID, el cual se encargará de verificar la dirección y sentido en corrección del servo-motor, dándonos una dirección y un tiempo de giro, para poder estabilizar la posición en las vueltas. Este es a base de los ángulos del giroscopio, para brindar estabilidad.
Tambien tenemos una secuencia para las esquinas, la cual detecta cuando una pared se acerca y coloca la secuencia de giro, dependiendo el sentido de giro que tenemos, a su vez, en detección automática de la tercera vuelta, se detiene en su zona de inicio, para poder dar fin a su recorrido.
El PID está basado en 4 partes, o 4 PID dependientes, estos serían:
Vuelta en sentido horario:
1.- El PID de distancias, mantiene a distancias iguales de las paredes el vehículo.
2.- El PID del ángulo, mantiene en una línea perpendicular a las paredes al vehículo.
Vuelta en sentido antihorario:
1.- El PID de distancias, mantiene a distancias iguales de las paredes el vehículo.
2.- El PID del ángulo, mantiene en una línea perpendicular a las paredes al vehículo.
Estos se encargan de enviar instrucciones con los valores al motor servo para girar y mantenerse recto hasta las secuencias de esquinas, al igual que el contador que se encuentra para detener el vehículo en la 4ta vuelta al inicio. El vehículo se controlo en su velocidad crucero del 80% de potencia y un 50% para las vueltas, por estabilidad.
