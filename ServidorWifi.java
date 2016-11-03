import java.net.*;
import java.io.*;
import javax.comm.ParallelPort;

public class ServidorWifi
{
	public static void main(String[] args)
	{
        ServerSocket serverSocket = null;
        Socket socket= null;

		String respuesta="";
        String comando="";
        String param1;
        String param2;

        int grados,sentido;
    
		try
		{
            //abrimos un socket, que escuchará en el puerto 1012
			System.out.println("Escuchando por el puerto 1012");
			serverSocket = new ServerSocket(1012); 
		}
		catch(IOException e)
		{
			System.out.println("java.io.IOException generada");
			e.printStackTrace();
		}

		System.out.println("Esperando a que los clientes se conecten...");
		while(true)
		{
			try
			{
                //acceptamos cualquier conexión, tomar en cuenta que para 
                //un tema de seguridad deberiamos agregar algún tipo de autenticación
				socket = serverSocket.accept();
				System.out.println("Se conecto un cliente: " + socket.getInetAddress().getHostName());

                //abrimos flujos de entrada y salida para la conexión
				DataInputStream dis = new DataInputStream( socket.getInputStream() );
				DataOutputStream dos = new DataOutputStream( socket.getOutputStream() );

                //Aquí esperamos la instrucción desde el cliente
				comando = dis.readUTF();
                
                //si el comando es válido inicia un intercambio de flujos
                if(comando.equals("1"))
                {
                    //solicitud del servidor del parametro grados
                    respuesta = "grados";
                    dos.writeUTF(respuesta);
                    //recibe el valor de los grados del cliente
                    grados = dis.readInt();
                    if(grados != 0) //si recibe un valor grados correcto solicita el sentido
                    {
                        //solicitud del servidor del parametro sentido
                        respuesta = "sentido";
                        dos.writeUTF(respuesta);
                        //recibe el valor de sentido.
                        sentido = dis.readInt();
                        //imprimimos los parametros recibidos
                        System.out.println("Parámetros: "+ sentido +"(sentido), "+ grados + " (grados)");
                        //hacemos los parametros Strings para poder pasarlos como tal
                        param1 = ""+grados;
                        param2 = ""+sentido;
                        //Llamamos al ejecutable y pasamos los parametros
                        Process proceso = new ProcessBuilder(".\\MoverMotor.exe",param1,param2).start();
                    }
                    
                }
                if(comando.equals("2")){
                    System.exit(0);
                }
                else //Si los parametros son inválidos manda este mensaje
                {   
                    respuesta = "Parametros incorrectos";
                    dos.writeUTF(respuesta);
                }

                //cerramos los flujos
                dos.close();
				dis.close();
                //cerramos la conexión
				socket.close();
                
            }
                
			catch(IOException e)
			{
				System.out.println("java.io.IOException generada");
				e.printStackTrace();
			}
		}
	}
}
