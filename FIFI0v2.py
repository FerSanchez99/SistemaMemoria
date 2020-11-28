#-------------------------Mateo Espinosa----------------------------#
#---------------------------A00823972-------------------------------#
#Primera parte del proyecto final de Sistemas Operativos.
#Se implementara el modelo FIFO para el manejador de memoria virtual.

#Memoria fisica = 2048 bytes.
#Cada pagina es de 16 bytes, tendriamos 128 paginas.

import math                 #Usamos para calcular el num de paginas (mat.floor())

ins = [0]                   #Inicializamos la entrada que se declara despues completa, esto es para poder entrar al WHILE()
memoria_procesos = []       #Inicializamos la QUEUE para nuestra memoria de procesos
memoria_real = 0            #Inicializamos la memoria real, su valor maximo es 128 y no se resetea para identificar cuando realizar un SWAP
swap_total = 0              #Inicializamos el valor del SWAP TOTAL, llevara la suma para poder hacer los insert en la QUEUE de manera correcta
indice_swap = 0             #Inicializamos varibale para evitar errores con el indice del swap_total cuando se da un doble swap
area_swap = []              #Inicializamos la QUEUE que tendra los procesos que entran al area de SWAP (MAXIMO 516 PAGINAS)
acumulador = 0              #Usado para desplegar el indice del area de swap
turnaround = {}             #Diccionario que llevara acabo el tracking del turnaround de cada proceso sin acumular
pagefaults = []             #Arreglo que llevara acabo el tracking de los page faults de cada proceso

while(ins[0] != 'E'):                       #'E' es para salir del programa

    instruccion = input()                   #Ingresamos nuestra instruccion al programa
    instruccion = instruccion.upper()       #Le normalizamos convirtiendole a mayusculas para evitar errores de sintaxis
    ins = instruccion.split()               #Separamos los tres parametros de nuestra instruccion
    
#------------------------------------------------------------------INICIA INSTRUCCION 'P'-------------------------------------------------------------------------------------------------------------------------------------------#    
    if(ins[0] == 'P'):                      #Entramos a la seccion de instrucciones P (meter procesos al QUEUE)

        num_bytes = int(ins[1])             #Transformamos a entero el numero de bytes para el proceso actual
        proceso_actual = int(ins[2])        #Transformamos a entero el proceso actual, se usa en instruccion de tipo 'P' y 'A' asi que la declaramos de manera global
        swap = 0                            #SWAP siempre debe iniciar en 0

        if(num_bytes > 2048):                                                   #Nos aseguramos que el tamano del proceso sea menor a 2048 bytes

            print("Memoria insuficiente para el proceso",proceso_actual)        #Print error y continuar con el siguiente proceso

        elif(num_bytes < 0):                                                    #Nos aseguramos que el numero sea positivo

            print("EL numero de bytes para cada proceso debe ser mayor a 0")    #Imprimimos error y continuamos

        else:

            num_paginas = math.floor(num_bytes/16)          #Calculamos el numero de paginas necesarias para el proceso actual
            if(num_paginas == 0):                           #Ya que math.floor() nos puede dar como resultado 0, nos aseguramos que el valor minimo sea 1
                num_paginas = 1

        #--------------------------------------SOLAMENTE SE ENTRARA A ESTA SECCION HASTA LLENAR POR PRIMERA VEZ LA MEMORIA-----------------------------------------------------------------------------------#

            print(ins[0],ins[1],ins[2])                                 #Imprimimos la instruccion sin ningu tipo de error de sintaxis como espacios blancos extra
            
            if(memoria_real < 128):                                     #Verificamos si el espacio disponible es suficiente o si es necesario hacer un SWAP en la primera corrida del programa

                if((memoria_real + num_paginas) < 128):                 #Verificamos si tenemos espacio en las 128 paginas iniciales para el proceso actual

                    for proceso in range(num_paginas):                  #Anadimos el proceso actual a la QUEUE
                        memoria_procesos.append(proceso_actual)
                        #OUTPUT DE LA INSTRUCCION P ANTES DE LLENAR LA MEMORIA POR PRIMERA VEZ.
                        print("Se asigno marco de pagina a memoria real",memoria_real)
                        memoria_real = memoria_real + 1                 #Aumentamos la memoria real

                    turnaround[proceso_actual] = num_paginas        #Metemos en el diccionario el numero de paginas con su respectivo proceso, ya que cada pagina equivale a 1 seg
                    for tiempo in turnaround:
                        if(tiempo == proceso_actual):               #Necesitamos ir sumando los turnaround de los procesos que estan esperando, por eso hay que usar un for para sumar los valores
                            break
                        else:
                            turnaround[tiempo] = turnaround.get(tiempo) + num_paginas

                else:
                    
                    swap = memoria_real + num_paginas - 128             #SWAP que se usara cuando sobrepasamos la memoria pero es la primera vez que se llena

                    for proceso in range(num_paginas - swap):           #El range es solamente del numero paginas que alcanzan en la memoria sin hacer SWAP
                        memoria_procesos.append(proceso_actual) 
                        #OUTPUT DE LA INSTRUCCION P ANTES DE LLENAR LA MEMORIA POR PRIMERA VEZ--------------------------
                        print("Se asigno marco de pagina a memoria real",memoria_real)   
                        #-------------------------------TERMINA OUTPU---------------------------------------------------     
                        memoria_real = memoria_real + 1                 #Seguimos incrementando la memoria real hasta que llegue a 128

                    for proceso in range(swap):                         #Hacemos el primer SWAP donde sacamos los primeros procesos ya que fueron los primeros en llegar  
                        area_swap.append(memoria_procesos[proceso])     #Metemos al area de swap al proceso que esta a punto de salir          
                        memoria_procesos.pop(proceso)
                        acumulador = acumulador + 1                     #Empezamos a sumar el indice del area de swap
                        #OUTPUT DE LA INSTRUCCION P PARA EL PRIMER SWAP--------------------------------------------------
                        print("SwapOut - Proceso:",area_swap[proceso],"Pagina:",proceso,"--al-> Marco de Swap:",proceso)
                        print("Se asigno marco de pagina a memoria real",proceso)
                        #------------------------------TERMINA OUTPUT----------------------------------------------------

                    for proceso in range(swap):                         #Metemos el proceso actual a la QUEUE
                        memoria_procesos.insert(proceso,proceso_actual)   

                    swap_total = swap_total + swap                      #Necesitamos tener el valor de SWAP_TOTAL para poder usarlo en el siguiente proceso

                    turnaround[proceso_actual] = num_paginas + (swap * 2)
                    for tiempo in turnaround:
                        if(tiempo == proceso_actual):               #Necesitamos ir sumando los turnaround de los procesos que estan esperando, por eso hay que usar un for para sumar los valores
                            break
                        else:
                            turnaround[tiempo] = turnaround.get(tiempo) + num_paginas + (swap * 2)

            #-----------------------------------DESPUES DE LLENAR LA MEMORIA DE 128 PAGINAS POR PRIMERA VEZ SE ENTRAR SIEMPRE AL ELSE-----------------------------------------#
            
            else:

                #DOBLE SWAP = Es cuando haces UN swap en la memoria, sobrepasas los 128 iniciales y tienes que hacer OTRO swap en las primeras posiciones.
                indice_swap = swap_total                                                    #Asi guardamos el valor de swap_total antes de verificar si swap_total + num_paginas > 128

                if((swap_total + num_paginas) <= 128):                                      #Si la condiciones TRUE, significa que tenemos suficiente espacio para meter el proceso en la memoria son un doble swap

                    for proceso in range(num_paginas):
                        area_swap.append(memoria_procesos[swap_total + proceso])                                    #Metemos al area de swap el proceso mas antiguo en memoria procesos
                        #OUTPUT PARA P DESPUES DE QUE SE LLENA LA MEMORIA POR PRIMERA VEZ Y AUN HAY ESPACIO SUFICIENTE PARA EL PROCESO ACTUAL
                        print("SwapOut - Proceso:",area_swap[len(area_swap) - 1],"Pagina:",(swap_total + proceso),"--al-> Marco de Swap:",(acumulador))
                        print("Se asigno marco de pagina a memoria real",(proceso  +swap_total))
                        #---------------------------TERMINA OUTPUT------------------------------------------------------------------------------
                        memoria_procesos.pop(swap_total + proceso)                                                  #swap_total + proceso nos dara el indice correcto ya que todavia hay espacio en memoria
                        memoria_procesos.insert((swap_total + proceso),proceso_actual)                              #En esta ocasion tenemos que sacar y meter procesos al mismo tiempo para evitar problemas con los indices
                        acumulador = acumulador + 1                                                                 #Sumamos el indice del indice del swap que sera mayor a 128

                    swap_total = swap_total + num_paginas                                                           #El swap_total es el numero de paginas acumulado ya que tenemos suficiente memoria para el proceso

                else: 

                    swap = num_paginas + swap_total - 128                                   #Siempre debe ser positivo, se usa para ver cuantas paginas se necesita depsues del primer SWAP

                    if((swap_total + num_paginas) < 128):                                   #Verificamos si el proceso actual sobrepasara las 128 paginas y actualizamos el SWAP_TOTAL
                        swap_total = swap_total + num_paginas
                    else:
                        swap_total = swap                                                   #El swap_total se vuelve igual al swap cuando ya necesitamos realizar el doble swap


                    for proceso in range(num_paginas - swap):                               #SWAP para completar las 128 paginas y despues iniciar el doble swap
                        area_swap.append(memoria_procesos[indice_swap + proceso])           #Metemos a la area de swap el proceso mas antiguo antes de iniciar el doble swap
                        #OUTPUT DE LA INSTRUCCION P PARA EL PRIMER SWAP--------------------------------------------------
                        print("SwapOut - Proceso:",area_swap[len(area_swap) - 1],"Pagina:",(proceso + indice_swap),"--al-> Marco de Swap:",(acumulador))
                        print("Se asigno marco de pagina a memoria real",(proceso + indice_swap))
                        #------------------------------TERMINA OUTPUT----------------------------------------------------
                        memoria_procesos.pop(indice_swap + proceso)                         #En este caso se nesita hacer pop() e insert() al mismo timepo para evitar problemas con los indices del arreglo
                        memoria_procesos.insert((indice_swap + proceso),proceso_actual)
                        acumulador = acumulador + 1

                    for proceso in range(swap):                                             #Reemplazamos las paginas faltantes en los marcos de pagina mas antiguos (LOS PRIMEROS EN INDICE 0 PARA ADELANTE)
                        area_swap.append(memoria_procesos[proceso])                         #Metemos los procesos antiguos despues del doble swap en los procesos mas antiguos (SERIAN LOS PRIMEROS)
                        #OUTPUT DE LA INSTRUCCION P PARA EL PRIMER SWAP--------------------------------------------------
                        print("SwapOut - Proceso:",area_swap[len(area_swap) - 1],"Pagina:",proceso,"--al-> Marco de Swap:",(acumulador))       #ES NECESARIO USAR INDICIE len(area_swap) - proceso - 1 PORQUE ESTAMOS USANDO APPEND PARA METER PROCESOS AL AREA DE SWAP
                        print("Se asigno marco de pagina a memoria real",proceso)
                        #------------------------------TERMINA OUTPUT----------------------------------------------------
                        memoria_procesos.pop(proceso)
                        memoria_procesos.insert(proceso,proceso_actual)
                        acumulador = acumulador + 1

                turnaround[proceso_actual] = num_paginas * 2
                for tiempo in turnaround:
                    if(tiempo == proceso_actual):               #Necesitamos ir sumando los turnaround de los procesos que estan esperando, por eso hay que usar un for para sumar los valores
                        break
                    else:
                        turnaround[tiempo] = turnaround.get(tiempo) + num_paginas * 2

#------------------------------------------------------------TERMINA INSTRUCCION 'P' E INICIA INSTRUCCION 'A'---------------------------------------------------------------------------------------------------------------------------------------------------#

    if(ins[0] == 'A'):                                                                          #Entramos a la instruccion de tipo 'A'

        print(ins[0],ins[1],ins[2],ins[3])                                                      #La primera parte del OUTPUT es la instruccion 
        proceso_actual = int(ins[2])                                                            #Transformamos a entero el proceso actual, se usa en instruccion de tipo 'P' y 'A' asi que la declaramos de manera global
        dir_virtual = int(ins[1])                                                               #Pasamos como entero a la direccion virtual que necesitamos acceder
        lectura_escritura = int(ins[3])                                                         #Si lectura_Escritura es 1, se lee y escribe el valor en         
        paginas_necesarias = math.floor(dir_virtual/16)                                         #Calculamos cuantas paginas esta desfasada del indice nuestro proceso                                                       
        indice_pagina = memoria_procesos.index(proceso_actual)                                  #Calculamos el primer indice del proceso actual para ver si esta en memoria

        if((proceso_actual not in memoria_procesos) and (proceso_actual not in area_swap)):     #Revisamos que el proceso se encuentre en alguna locacion de memoria real o area de swap
            
            print("El proceso",proceso_actual,"no se encuentra en memoria ni en area de swap.")
            
        elif((proceso_actual in memoria_procesos) and (proceso_actual not in area_swap)):       #Solamente si proceso esta dentro de memoria pero no en area de swap

            memoria = (indice_pagina * 16) + dir_virtual                                        #Asi calculamos la meoria real en bytes (SIN CONTAR EL DOBLE SWAP TODAVIA)

            if(lectura_escritura == 0):                                                         #Tenemos que analizar si es escritura o lectura, y dependiendo de eso tenemos un output diferente
                #OUTPUT INSTRUCCION 'A' CUANDO EL PROCESO ESTA SOLO EN MEMORIA REAL Y NO EN AREA DE SWAP
                print("Obtener la dirección real correspondiente a la dirección virtual",dir_virtual,"del proceso",proceso_actual)
                print("Direccion real -->",memoria)
                print("Dirección virtual -->",dir_virtual)
            else:
                #OUTPUT INSTRUCCION 'A' CUANDO EL PROCESO ESTA SOLO EN MEMORIA REAL Y NO EN AREA DE SWAP
                print("Obtener la dirección real correspondiente a la dirección virtual",dir_virtual,"del proceso",proceso_actual)
                print("Pagina",paginas_necesarias,"del proceso",proceso_actual,"modificada.")
                print("Direccion real -->",memoria)
                print("Dirección virtual -->",dir_virtual)

            turnaround[proceso_actual] = round((float(turnaround.get(proceso_actual)) + 0.1),1)     #Aqui calculamops el turnaround time cuando se accesa o se modifica la pagina, usamos round para evitar tener muchos decimales (ACCESO A MEMPORIA ES DE 0.1 s)
            for tiempo in turnaround:
                if(tiempo == proceso_actual):               #Necesitamos ir sumando los turnaround de los procesos que estan esperando, por eso hay que usar un for para sumar los valores
                    break
                else:
                    turnaround[tiempo] = float(turnaround.get(tiempo)) + 0.1

        elif((proceso_actual not in memoria_procesos) and (proceso_actual in area_swap)):

            indice_en_swap = area_swap.index(proceso_actual)                                        #Aqui buscamos al primer indice del proceso que estamos buscando en area de swap            
            pagina_que_buscamos = indice_en_swap + paginas_necesarias                               #Asi podemos encontrar la pagina que queremos y usaremos esta para meterla a la memoria de procesos

            if(swap_total == 128):                                                                  #Como no se hace esat evaluacion en esta instruccion, es necesario reiniciar el swap total
                swap_total = 0

            memoria_procesos.pop(swap_total)                                                        #Aqui sacamos el proceso mas antiguo de la memoria para posteriormente insertar el proceso que esta en area swap
            memoria_procesos.insert(swap_total,area_swap[pagina_que_buscamos])                      #Insertamos el proceso que esta en area swap a la memoria
            area_swap.pop(pagina_que_buscamos)                                                      #Sacamos el proceso del area de swap

            if(lectura_escritura == 0):                                                             #Tenemos que analizar si es escritura o lectura, y dependiendo de eso tenemos un output diferente
                #OUTPUT INSTRUCCION 'A' CUANDO EL PROCESO SE ENCUENTRA EN AREA DE SWAP Y NO EN MEMORIA REAL
                print("Obtener la dirección real correspondiente a la dirección virtual",dir_virtual,"del proceso",proceso_actual)
                print("Se localizó la página",paginas_necesarias,"del proceso",proceso_actual,"que estaba en la posición",pagina_que_buscamos,"de swapping")
                print("Se la cargo al marco -->",swap_total)
            else:
                print("Obtener la dirección real correspondiente a la dirección virtual",dir_virtual,"del proceso",proceso_actual)
                print("Se localizó la página",paginas_necesarias,"del proceso",proceso_actual,"que estaba en la posición",pagina_que_buscamos,"de swapping")
                print("Se la cargo al marco -->",swap_total)
                print("Pagina",paginas_necesarias,"del proceso",proceso_actual,"modificada.")

            turnaround[proceso_actual] = round(float(turnaround.get(proceso_actual)) + 2.1, 1)      #Tenemos que sumar 2.1 s ya que necesitaremos hacer swap de 1 pagina y aparte suma el 0.1 s del accesamiento a la pagina
            for tiempo in turnaround:
                if(tiempo == proceso_actual):               #Necesitamos ir sumando los turnaround de los procesos que estan esperando, por eso hay que usar un for para sumar los valores
                    break
                else:
                    turnaround[tiempo] = float(turnaround.get(tiempo)) + 2.1

        elif((proceso_actual in memoria_procesos) and (proceso_actual in area_swap)): 
        
            proceso_en_memoria = 0                                                          #Inicializamos la variable local que tendra cuantos procesos actuales tenemos en memoria real
            proceso_en_swap = 0                                                             #Inicializamos la variable local que tendra cuantos procesos actuales tenemos en area de swap

            for proceso in list(memoria_procesos):
                if(proceso_actual == proceso):
                    proceso_en_memoria = proceso_en_memoria + 1                             #Actualizamos el valor proceso_en_memoria

            for proceso in list(area_swap):
                if(proceso == proceso_actual):
                    proceso_en_swap = proceso_en_swap + 1                                   #Actualizamos el valor de proceso_en_swap
                
            if(paginas_necesarias < proceso_en_memoria):
                memoria = (indice_pagina * 16) + dir_virtual                                #Asi calculamos la meoria real en bytes (SIN CONTAR EL DOBLE SWAP TODAVIA)

                if(lectura_escritura == 0):                                                 #Tenemos que analizar si es escritura o lectura, y dependiendo de eso tenemos un output diferente
                    #OUTPUT INSTRUCCION 'A' CUANDO EL PROCESO ESTA SOLO EN MEMORIA REAL Y NO EN AREA DE SWAP
                    print("Obtener la dirección real correspondiente a la dirección virtual",dir_virtual,"del proceso",proceso_actual)
                    print("Direccion real -->",memoria)
                    print("Dirección virtual -->",dir_virtual)
                else:
                    #OUTPUT INSTRUCCION 'A' CUANDO EL PROCESO ESTA SOLO EN MEMORIA REAL Y NO EN AREA DE SWAP
                    print("Obtener la dirección real correspondiente a la dirección virtual",dir_virtual,"del proceso",proceso_actual)
                    print("Pagina",paginas_necesarias,"del proceso",proceso_actual,"modificada.")
                    print("Direccion real -->",memoria)
                    print("Dirección virtual -->",dir_virtual)

                turnaround[proceso_actual] = round(float(turnaround.get(proceso_actual)) + 0.1, 1)      #Tenemos que sumar 2.1 s ya que necesitaremos hacer swap de 1 pagina y aparte suma el 0.1 s del accesamiento a la pagina
                for tiempo in turnaround:
                    if(tiempo == proceso_actual):               #Necesitamos ir sumando los turnaround de los procesos que estan esperando, por eso hay que usar un for para sumar los valores
                        break
                    else:
                        turnaround[tiempo] = float(turnaround.get(tiempo)) + 0.1

            else:
                
                if(paginas_necesarias < proceso_en_memoria):
                    pagina_que_buscamos = paginas_necesarias - proceso_en_swap                          #Asi podemos encontrar la pagina que queremos y usaremos esta para meterla a la memoria de procesos
                else:
                    pagina_que_buscamos = paginas_necesarias - proceso_en_memoria                       #Si paginas necesarias es mayor al numero de procesos actuales en memoria debemos buscarlo en area de swap                   

                memoria_procesos.pop(swap_total)                                                        #Aqui sacamos el proceso mas antiguo de la memoria para posteriormente insertar el proceso que esta en area swap
                memoria_procesos.insert(swap_total,area_swap[pagina_que_buscamos])                      #Insertamos el proceso que esta en area swap a la memoria
                area_swap.pop(pagina_que_buscamos)                                                      #Sacamos el proceso del area de swap

                if(lectura_escritura == 0):                                                             #Tenemos que analizar si es escritura o lectura, y dependiendo de eso tenemos un output diferente
                    #OUTPUT INSTRUCCION 'A' CUANDO EL PROCESO SE ENCUENTRA EN AREA DE SWAP Y NO EN MEMORIA REAL
                    print("Obtener la dirección real correspondiente a la dirección virtual",dir_virtual,"del proceso",proceso_actual)
                    print("Se localizó la página",paginas_necesarias,"del proceso",proceso_actual,"que estaba en la posición",pagina_que_buscamos,"de swapping")
                    print("Se la cargo al marco -->",swap_total)
                else:
                    #OUTPUT INSTRUCCION 'A' CUANDO EL PROCESO SE ENCUENTRA EN AREA DE SWAP Y NO EN MEMORIA REAL
                    print("Obtener la dirección real correspondiente a la dirección virtual",dir_virtual,"del proceso",proceso_actual)
                    print("Se localizó la página",paginas_necesarias,"del proceso",proceso_actual,"que estaba en la posición",pagina_que_buscamos,"de swapping")
                    print("Se la cargo al marco -->",swap_total)
                    print("Pagina",paginas_necesarias,"del proceso",proceso_actual,"modificada.")
                
                turnaround[proceso_actual] = round(float(turnaround.get(proceso_actual)) + 2.1, 1)      #Tenemos que sumar 2.1 s ya que necesitaremos hacer swap de 1 pagina y aparte suma el 0.1 s del accesamiento a la pagina
                for tiempo in turnaround:
                    if(tiempo == proceso_actual):               #Necesitamos ir sumando los turnaround de los procesos que estan esperando, por eso hay que usar un for para sumar los valores
                        break
                    else:
                        turnaround[tiempo] = float(turnaround.get(tiempo)) + 2.1

#------------------------------------------------------------TERMINA INSTRUCCION 'A' E INICIA INSTRUCCION 'L'---------------------------------------------------------------------------------------------------------------------------------------------------#

    if(ins[0] == 'L'):                                                                              #Entramos a la seccion de instruccion L  

        proceso_actual = int(ins[1])                                                                #Transformamos a entero el proceso actual que se encuentra en la posicion [1] de la instruccion
        print(ins[0],ins[1])                                                                        #La primera parte del OUTPUT es la instruccion 

        if(len(memoria_procesos) == 0):                                                             #Comprobamos que exista procesos dentro de la memoria

            print("No existe procesos en memoria")

        elif((proceso_actual in memoria_procesos) or (proceso_actual in area_swap)):                #Comprobamos que el proceso actual se encuentre dentro de la memoria        

            print("Liberar los marcos de página ocupados por el proceso",proceso_actual)            #La segunda nos dice que proceso sera el que sera liberado

            if(proceso_actual in memoria_procesos):                                                 #Revisamo si el proceso actual se enuentra en la memoria de procesos
                indice_memoria = memoria_procesos.index(proceso_actual)                             #Declaramos el indice del primer proceso actual en memoria 
                acumulador_memoria = indice_memoria                                                 #Este acumulador llevara el marco de pagina que se libero
                
                for proceso in list(memoria_procesos):                                              #Empezamos a revisar si el proceso en la lista es igual al proceso actual
                    if(proceso == proceso_actual):
                        memoria_procesos.pop(indice_memoria)                                        #Siempre eliminamos el proceso con el primer indice ya que a la final eliminaremos todos
                        print("Se libera el marco de memoria real",acumulador_memoria)
                        acumulador_memoria = acumulador_memoria + 1                                 #Acumulador es la variable que lleva la cuenta de marcos de pagina liberados

            if(proceso_actual in area_swap):                                                        #Tenemos que revisar que exista el proceso dentro del area de swap
                if(len(area_swap) != 0):                                                            #Es necesario checar que exista en area swap algun valor
                    indice_en_areaswap = area_swap.index(proceso_actual)                            #Declaramos el indice del primer proceso actual en swap
                    acumulador_swap = indice_en_areaswap                                            #Este acumular llevar el marco de swap que se libero

                    for proceso in list(area_swap):                                                 #Empezamos a revisar si el proceso en la lista es igual al proceso actual
                        if(proceso == proceso_actual):
                            area_swap.pop(indice_en_areaswap)                                       #Siempre eliminamos el proceso con el primer indice ya que a la final eliminaremos todos
                            print("Se libera el marco de area de swap",acumulador_swap)
                            acumulador_swap = acumulador_swap + 1                                   #Acumulador es la variable que lleva la cuenta de marcos de pagina liberados

            memoria_real = len(memoria_procesos)                                                    #Tenemos que volver a calcular el valor de memoria real ya que debemos ser capaces de poder meter nuevos procesos a las zonas liberadas

        else:
            print("El proceso",proceso_actual,"no se encuentra en la memoria")
            
 #------------------------------------------------------------TERMINA INSTRUCCION 'L' E INICIA INSTRUCCION 'F'---------------------------------------------------------------------------------------------------------------------------------------------------#               

    if(ins[0] == 'F'):                                                                              #Inicia la instruccion F (REPORTE)

        print(ins[0])                                                                               #El primer output es la misma instruccion

        print("Reporte de salida:")
        print("EL Turnaround Time es:",turnaround)
        print("El numero de PageFaults es:",pagefaults)
        print(turnaround)

 #------------------------------------------------------------TERMINA INSTRUCCION 'F'---------------------------------------------------------------------------------------------------------------------------------------------------#               