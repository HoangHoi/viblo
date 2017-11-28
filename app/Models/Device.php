<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;
use Illuminate\Auth\Authenticatable;
use Illuminate\Foundation\Auth\Access\Authorizable;
use Illuminate\Contracts\Auth\Authenticatable as AuthenticatableContract;
use Illuminate\Contracts\Auth\Access\Authorizable as AuthorizableContract;
use App\Models\Traits\Activeable;
use App\Models\Traits\Filterable;

class Device extends Model implements
    AuthenticatableContract,
    AuthorizableContract
{
    use Authenticatable, Authorizable, Activeable, Filterable;

    protected $fillable = [
        'identify_code', 'name', 'password', 'is_actived',
    ];

    protected $table = 'devices';

    protected $hidden = [
        'password', 'remember_token',
    ];
}
