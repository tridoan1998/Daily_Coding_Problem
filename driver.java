package test;


import java.util.*;
import java.io.*;

public class driver {

	
	
	
	//problem#82
	public List<Character> readN(int n, List<Character> list)
	{
		
		if(n > list.size())
		{
			return list;
		}
		
		if(n == list.size())
		{
			return list;
		}
		else
		{
			List<Character> newlist = new ArrayList<Character>();
			
			
			for(int i = 0 ;i < n ; i++)
			{

				newlist.add(list.get(i));
			}
			
			for(int i = 0; i < 7; i++)
			{
				list.remove(0);
				if(list.size() == 0)
				{
					System.out.println("here");
					return null;
				}
			}
			
			if(newlist.size() < 7)
			{

			}
			return newlist;
		}
	}
	public static void main(String[] args) {

		
		
		driver d = new driver();
		try {
			
			File file=new File("file.txt");    //creates a new file instance  
			FileReader fr=new FileReader(file);   //reads the file  
			BufferedReader br=new BufferedReader(fr);  //creates a buffering character input stream  

			String line;
			line = br.readLine();
			
			char[] sringtochararray = line.toCharArray();
			List<Character> list = new ArrayList<Character>();

			for(int i = 0; i < sringtochararray.length; i++)
			{
				list.add(sringtochararray[i]);
			}
			System.out.println(list);
			System.out.println(d.readN(7, list));


			System.out.println(d.readN(7, list));


	
		}catch(FileNotFoundException e )
		{
			System.out.println("file cannot be found!");
		} catch (IOException e) {

			e.printStackTrace();
		}
	}

}
