import java.net.*;
import java.io.*;
import java.util.Scanner;

public class ClienteWifi
{
	public static void main(String[] args)

	{
        Socket socket = null;

        int grados,sentido;
        int puerto, opc;

        String respuesta="";
        String direccion ="";
        String comando="";
        
        //buffers de lectura de datos
        BufferedReader br = new BufferedReader( new InputStreamReader( System.in ) );
        Scanner sc= new Scanner(System.in);

		try
		{
            System.out.println("Programa cliente para controlar un motor a pasos mediante una conexión WiFi\n");
            System.out.println("\nIntroduce el nombre o IP de tu equipo");
            direccion = br.readLine();
            System.out.println("\nIntroduce el puerto");
            puerto = sc.nextInt();
            System.out.println("\nConectando al puerto: " +puerto+ " del servidor: " + direccion);
  
            do
            {
            //hacemos la conexión al servidor
            socket = new Socket(direccion, puerto); 
            System.out.println("\n\n");
            System.out.println("\nSelecciona alguno de los comandos disponibles\n\n");
            System.out.println("\t1. Mover motor");
            System.out.println("\t2. Salir");
            System.out.println("\n");
            //leemos el comando
            comando = br.readLine();
            
            System.out.println("\nEnviando peticion: " +comando);
            
            System.out.println("\n");

            //abrimos flujos de entrada y salida para la conexión
            DataOutputStream dos = new DataOutputStream( socket.getOutputStream() );
            DataInputStream dis = new DataInputStream( socket.getInputStream() );
                           
            //envia el comando al servidor
            dos.writeUTF(comando);
            
            //recibimos la respuesta del servidor
            respuesta=dis.readUTF();
            
            //comparación para saber si se solicitan los grados
            if(respuesta.equals("grados"))
            {
                System.out.println("Introduce los grados:\n");
                grados = sc.nextInt();
                //envia los grados al servidor
                dos.writeInt(grados);
                //recibimos respuesta del servidor
                respuesta = dis.readUTF();

                //comparación para saber si solicita la variable sentido
                if (respuesta.equals("sentido")) 
                {   
                    System.out.println("Introduce el sentido (0 antihorario, 1 horario):");
                    sentido = sc.nextInt();
                    //envia el valor sentido al servidor
                    dos.writeInt(sentido);   
                }
                
            }
            //Cierre de flujos
            dos.close();
            dis.close();
            //Cierre de la conexión
            socket.close();
                
            
            System.out.println("\n\n\tSi deseas realizar alguna otra petición, presiona 1");
            opc = sc.nextInt();
            
                
            }while(opc==1);     //cierre del do-while
            
		}//cierre del try

		catch(IOException e)
		{
			System.out.println("java.io.IOException generada");
			e.printStackTrace();
		}
	}
}