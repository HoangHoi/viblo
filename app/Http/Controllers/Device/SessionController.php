<?php

namespace App\Http\Controllers\Device;

use App;
use Auth;
use Illuminate\Http\Request;
use App\Http\Controllers\Controller;
use Tymon\JWTAuth\JWTAuth;
use App\Core\Responses\Auth\AuthResponse;

class SessionController extends Controller
{
    protected $guard = 'device';

    public function __construct()
    {
        $this->middleware('guest:device', ['except' => ['logout', 'index']]);
        $this->middleware('auth:device', ['only' => ['logout']]);
    }

    public function session(JWTAuth $auth)
    {
        if (Auth::guard($this->guard)->check()) {
            return $this->userWasAuthenticated($auth);
        }

        return $this->responseNotLogin();
    }

    public function login(Request $request, JWTAuth $auth)
    {
        $this->validateLogin($request);
        $credentials = $this->getCredentials($request);

        if ($this->attemptLogin($request, $credentials)) {
            return $this->userWasAuthenticated($auth);
        }

        return AuthResponse::loginFailedResponse();
    }

    public function logout()
    {
        Auth::guard($this->guard)->logout();

        return AuthResponse::logoutSuccessResponse();
    }

    protected function attemptLogin(Request $request, $credentials)
    {
        return Auth::guard($this->guard)->attempt($credentials, true);
    }

    protected function validateLogin(Request $request)
    {
        $this->validate($request, [
            'identify_code' => 'required|string',
            'password' => 'required|string',
        ]);
    }

    protected function getCredentials(Request $request)
    {
        return $request->only('identify_code', 'password');
    }

    protected function userWasAuthenticated($auth)
    {
        $user = Auth::guard($this->guard)->user();
        if ($user->isActive()) {
            $authToken = $auth->fromUser($user);
            $result = $this->makeUserResult($user, $authToken);

            return $this->loginSuccessResponse($result);
        }

        $this->logout();
        return $this->userNotActiveResponse();
    }

    protected function makeUserResult($user, $authToken)
    {
        return [
            'id' => $user->id,
            'identify_code' => $user->identify_code,
            'auth_token' => $authToken,
            'guard' => $this->guard,
        ];
    }

    protected function loginSuccessResponse($data)
    {
        $data['status'] = 'login_in';
        $data['token'] = csrf_token();
        return $this->response($data);
    }

    protected function userNotActiveResponse()
    {
        return $this->response([
            'status' => 'not_active',
            'token' => csrf_token(),
        ]);
    }

    protected function responseNotLogin()
    {
        return $this->response([
            'status' => 'not_login',
            'token' => csrf_token(),
        ]);
    }

    protected function responseToFailedLogin()
    {
        return $this->response([
            'status' => 'login_fail',
            'token' => csrf_token(),
        ]);
    }

    protected function response($data, $statusCode = 200)
    {
        return response()->json($data, $statusCode);
    }
}
