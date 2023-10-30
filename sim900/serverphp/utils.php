<?php
  //Abrir conexion a la base de datos
  function connect()
  {
   $dbhost = 'qagm691.hostpablodev.com:3036';
   $dbuser = 'qagm691';
   $dbpass = 'qwe159ZXC';
   $database = "qagm691";
   $conn = mysql_connect($dbhost, $dbuser, $dbpass, $database);
   
   if(! $conn ) {
      die('Could not connect: ' . mysql_error());
   }
   
   echo 'Connected successfully';
  }

 //Obtener parametros para updates
 function getParams($input)
 {
    $filterParams = [];
    foreach($input as $param => $value)
    {
            $filterParams[] = "$param=:$param";
    }
    return implode(", ", $filterParams);
	}
  //Asociar todos los parametros a un sql
	function bindAllValues($statement, $params)
  {
		foreach($params as $param => $value)
    {
				$statement->bindValue(':'.$param, $value);
		}
		return $statement;
   }
 ?>