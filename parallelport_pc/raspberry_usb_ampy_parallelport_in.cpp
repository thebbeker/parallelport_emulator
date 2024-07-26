int parallelport_in() { 

// 
//  2024-03-24  Thomas Hebbeker
//  2024-04-16  version 2.0   cleanup 
//
//
//  get the 5 status line bits, encoded into one byte, of the parallel port 
//    via USB and Raspberry pico
// 
//  note: here cerr instead of cout is used for compatibility with parallelport_in (for raspberry) 
// 
	

  int statusbyte;
  int statusbit;
  int factor; 
   
  static bool first = true;
  const bool debug = false;

  string digit;
  string command;
   
  const char* usbport = RASPBERRY_USBPORT; 
  string usb;
  usb = usbport;
  
  static int gpio[5] = {18,19,20,21,22}; 
  
  static ofstream file_read[5];
  
  static string filename_read[5];
  
  ifstream stdout_file; 
   
   if(first) {
    cerr << " >>> parallelport_in <<<  raspberry_usb    ampy   2024-04-16   T.Hebbeker  " <<  endl;

     for(int i=0; i<5; i++) {
       digit = to_string(i); 
       filename_read[i] = "file_read_" + digit + ".py";
       digit = to_string(gpio[i]); 
       file_read[i].open(filename_read[i],ios::out);  
       file_read[i] << "from machine import Pin" << endl;
       file_read[i] << "gpiopin = Pin("+ digit + ",Pin.IN)" << endl; 
       file_read[i] << "print(gpiopin.value())" << endl; 
       file_read[i].close();
    }
    first = false;
  }
	
	
  if(debug) cerr << " >>> parallelport_in <<<  DEBUG  " << endl; 
  
  
  statusbyte = 0; 
  factor = 1;
  
     // return statusbyte;

  for(int i=0; i<5; i++) {
	  
	   FILE *stdout_pico = freopen("stdout_file.txt","wb",stdout);       //  redirect cout to file on host PC  - for parallelport_in
	
   command = " ampy --port " + usb + " run " + filename_read[i];
   system(command.c_str());
	   
  usleep(100000); // wait a while 

	fclose(stdout_pico); 
	
	// here read output file written by raspberry pico =  stdout_file.txt 
	usleep(100000); // wait a while 

	statusbit = 0; 
    stdout_file.open("stdout_file.txt");
    stdout_file >> statusbit;
	  stdout_file.close();
  
	
    if(debug) cerr << " for command " << command << "    global read.  raspberry pico gives = " << statusbit << endl; 

    //  return statusbyte;  // does not return to calling main 

	
	 statusbyte = statusbyte + statusbit * factor;
	 factor = factor*2;
	 
    usleep(100000); // wait a while 
	  
  }  // end for i
  
   if(debug) cerr << " leaving for i loop, statusbyte = " << statusbyte << endl; 

   return statusbyte;

	
}  // end function 
