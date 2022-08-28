<?php
    require_once $_SERVER['DOCUMENT_ROOT'].'/top.php';
    include_once $_SERVER['DOCUMENT_ROOT'].'/header.php';
?>

<!--
<div class=WriteDivStyle>
    <a href="http://<?=$_SERVER['HTTP_HOST']?>/write.php" class=Writebtn>글쓰기</a>
</div>
<div class=WriteDivStyle>
    <a href="http://<?=$_SERVER['HTTP_HOST']?>/list.php" class=Writebtn>목록</a>
</div>
<br><br><br>
-->

<form name="write_form" action="./write_process.php" method="POST">
    <?php
        if($_SESSION['is_signin'] == 'y')
        {
            echo '<input type ="hidden" name="member_idx" value="<?=$_SESSION[\'member_idx\']?>">';
            echo '<input type ="hidden" name="writer" value="<?=$_SESSION[\'writer\']?>">';
        }
        else
        {
            //echo '<input type ="hidden" name="member_idx" value="0">';
            //header("Location: ".$url['root'].'signin.php');
            //exit();

        }
    ?>
    <table class=WriteTableStyle>
        <tr class=WirteTrStyle>
            <th class=WriteTdStyle>제목</td>
            <td class=WriteTdStyle><input type="text" name="subject" size="100" style="width: 100%; height: 40px;" autofocus required></td>
        </tr>
        <tr class=WirteTrStyle>
            <th class=WriteTdStyle>내용</td>
            <td class=WriteTdStyle><textarea name="content" cols="100" rows="10"></textarea></td>
        </tr>
    </table>

    <div><input type="submit" value="저장"></div>
</form>

<?php
    include_once($_SERVER['DOCUMENT_ROOT']."/footer.php");
?>