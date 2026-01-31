# LOG 


## DEC-09

Make the handler handle all I/O so that the prompt from the function called only does the operation, hence have the functions input handleded centrally somewhere with the state handler so that the printout from the function is merged with the "to go back press b" and the handler can expect input from both function specific and also the "b". 

## DEC-10

Kanske jobbigt att få alla meddelanden från funktionerna igenom en och samma handler, kanske bättre att splitta upp så att varje funktion har sin egen handler med i/o.