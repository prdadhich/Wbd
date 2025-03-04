# Wbd
Sample json file structure 
.....Start
{
  "StartLocation": {
    "X": 150.0,
    "Y": -350.0,
    "Z": 100.0
  },
  "TargetLocation": {
    "X": 150.0,
    "Y": -350.0,
    "Z": 400.0
  },


  "Textures": {
    "wood": "C:/Users/prdad/Downloads/abstract/OC1Y8P0.jpg",
    "metal": "C:/Users/prdad/Downloads/grid/square.jpg"
    
  }
}
......end



in blueprint actor add the moveactor component
![image](https://github.com/user-attachments/assets/f2fe5377-df9b-46bf-85f4-328e548cc088)




calling the functions
startmove : will move the respective blueprint actor targetlocation in json

startmovetostart: will move the actor to startlocation in json

applytexturetomesh: applies the texture to mesh from json, need to pass the key to pass which texture needs to applied

![image](https://github.com/user-attachments/assets/82262b08-3692-485c-a7b8-80aea0a6f0e0)
