<!doctype html>
<html>
    <head>
        <title>Victim</title>
        <meta charset="utf-8">
        <link href="./style.css?after" rel="stylesheet" type="text/css">
    
    </head>
    <body>
        <div class="header">
        <?php
            if($_SESSION['is_signin'] != 'y')
            {
                //$_SESSION['is_signin'] = 'n';
                echo "로그인 해주세요.<br>";
                echo "Unknown님 로그인이 되어 있지 않습니다.";
                $_SESSION['member_idx'] = 0;
            }
            else
            {
                echo $_SESSION["user_id"]."님, 로그인 되었습니다.";
                echo "<br>";
            }
        ?>
        </div>
        <div class="content">
            <div class=IndexDivStyle>
                <a href="signin.php" class="btn">Login</a>
            </div>

            <?php
                if($_SESSION['is_signin'] == 'y')
                {
                    echo '
                    <div class=IndexDivStyle>
                        <a href="http://'.$_SERVER['HTTP_HOST'].'/logout.php" class="btn">Logout</a>
                    </div>
                    ';
                }
            ?>

            <div class=IndexDivStyle>
                <a href="signup.php" class="btn">Register</a>
            </div>

            <div class=IndexDivStyle>
                <a href="write.php" class="btn">Write</a>
            </div>

            <div class=IndexDivStyle>
                <a href="list.php" class="btn">List</a>
            </div>

            <br><br><br><br>
