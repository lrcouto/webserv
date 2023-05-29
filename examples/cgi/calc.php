<!DOCTYPE html>
<html>
<head>
  <title>Simple Calculator</title>
</head>
<body>
  <h1>Simple Calculator</h1>
  <form method="POST" action="">
    <input type="number" name="num1" placeholder="Enter number 1" required><br><br>
    <input type="number" name="num2" placeholder="Enter number 2" required><br><br>
    <select name="operator">
      <option value="add">Addition (+)</option>
      <option value="subtract">Subtraction (-)</option>
      <option value="multiply">Multiplication (*)</option>
      <option value="divide">Division (/)</option>
    </select><br><br>
    <input type="submit" value="Calculate">
  </form>
  <?php

  if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $requestBody = getenv('REQUEST_BODY');
    parse_str($requestBody, $requestData);

    $num1 = $requestData['num1'];
    $num2 = $requestData['num2'];
    $operator = $requestData['operator'];

    switch ($operator) {
      case 'add':
        $result = $num1 + $num2;
        break;
      case 'subtract':
        $result = $num1 - $num2;
        break;
      case 'multiply':
        $result = $num1 * $num2;
        break;
      case 'divide':
        if ($num2 != 0) {
          $result = $num1 / $num2;
        } else {
          echo '<p>Error: Division by zero is not allowed.</p>';
        }
        break;
      default:
        echo '<p>Error: Invalid operator.</p>';
        break;
    }

    if (isset($result)) {
      echo '<p>Result: ' . $result . '</p>';
    }
  }

  ?>
<br>
<a href="http://localhost:3007/cgi/what_is_cgi.html">Return</a>
</body>
</html>
