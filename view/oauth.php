<?PHP

class Sync
{
    public $appId = "2882303761517489490";
    public $ApiKey = 'Mf5KCPQDptxr81amSe2eliwz';
    public $SecretKey = 'h4RnbaVulUixyLjP5LxhNwai95QGqPrp';
    public $redirect_uri = 'http://mtjo.net/baiduSync/index.html';

    //参数1：访问的URL，参数2：post数据(不填则为GET)，参数3：提交的$cookies,参数4：是否返回$cookies
    static function curl_request($url,$post='',$cookie='', $returnCookie=0){

        $curl = curl_init();
        curl_setopt($curl, CURLOPT_URL, $url);
        curl_setopt($curl, CURLOPT_USERAGENT, 'Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/6.0)');
        curl_setopt($curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt($curl, CURLOPT_AUTOREFERER, 1);
        curl_setopt($curl, CURLOPT_REFERER, "http://XXX");
        if($post) {
            curl_setopt($curl, CURLOPT_POST, 1);
            curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($post));
        }
        if($cookie) {
            curl_setopt($curl, CURLOPT_COOKIE, $cookie);
        }

        curl_setopt($curl, CURLOPT_HEADER, $returnCookie);
        curl_setopt($curl, CURLOPT_TIMEOUT, 10);
        curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
        $data = curl_exec($curl);
        if (curl_errno($curl)) {
            return curl_error($curl);
        }
        curl_close($curl);
        if($returnCookie){
            list($header, $body) = explode("\r\n\r\n", $data, 2);
            preg_match_all("/Set\-Cookie:([^;]*);/", $header, $matches);
            $info['cookie']  = substr($matches[1][0], 1);
            $info['content'] = $body;
            return $info;
        }else{
            return $data;
        }
    }

    public function refresh_token($refresh_token)
    {
        $url = 'https://openapi.baidu.com/oauth/2.0/token';

        $postFields = array(
            'grant_type' => 'refresh_token',
            'refresh_token' => $refresh_token,
            'client_id' => $this->ApiKey,
            'client_secret' => $this->SecretKey
        );
        $response_r = $this->curl_request($url,$postFields);

        return $response_r;
    }

    public function bindaccount()
    {
        $location = 'https://openapi.baidu.com/oauth/2.0/authorize?response_type=code&' . 'client_id=' . $this->ApiKey . '&redirect_uri=' . $this->redirect_uri . '&scope=basic,netdisk' . '&display=mobile';
        header('Location:' . $location, true, 302);
    }

    public function getPcsToken($code)
    {
        $url = 'https://openapi.baidu.com/oauth/2.0/token';
        $postFields = array(
            'grant_type' => 'authorization_code',
            'code' => $code,
            'client_id' => $this->ApiKey,
            'client_secret' => $this->SecretKey,
            'redirect_uri' => $this->redirect_uri,
            'display' => 'dialog'
        );
        $response_r = $this->curl_request($url, $postFields);
        return $response_r;
    }

    public function getQuota($access_token)
    {
        $quota_url = "https://pcs.baidu.com/rest/2.0/pcs/quota?method=info&access_token=" . $access_token;
        $response_r = $this->curl_request($quota_url);
        $tmp_data = json_decode($response_r, 1);
        foreach ($tmp_data as &$v) {
            $v = sprintf("%.2f", $v / (1024 * 1024 * 1024));
        }
        return json_encode($tmp_data);
    }
}

if ($_REQUEST ['method'] && $_REQUEST ['method'] != "") {
    $method = trim($_REQUEST ['method']);
    $sync = new Sync ();
    // var_dump($sync);exit;
    switch ($method) {
        case "refresh_access_token" :
            if ($_REQUEST ['refresh_token'] != '') {
                //print_r($_REQUEST);
                die ($sync->refresh_token($_REQUEST ['refresh_token']));
            } else {
                echo "参数错误!";
            }
            break;
        case "bindAccount" :
            if ($_REQUEST ['bindAccount'] == 'mtjo') {
                $sync->bindaccount();
                exit ();
            } else {
                echo "参数错误!";
            }
            break;
        case "getPcsToken" :
            if ($_REQUEST ['code'] != '') {
                die ($sync->getPcsToken($_REQUEST ['code']));
            } else {
                echo "参数错误!";
            }
            break;
        case "getAppid" :
            $data ["appId"] = $sync->appId;
            die (json_encode($data));
            break;

        case "getQuota" :
            if ($_REQUEST ['access_token'] != '') {
                die ($sync->getQuota($_REQUEST ['access_token']));
            } else {
                echo "参数错误!";
            }
            break;
        default :
            echo "参数错误!";
            break;
    }
} else {
    echo "参数错误!";
}
?>



