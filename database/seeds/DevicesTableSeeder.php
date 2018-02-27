<?php

use Illuminate\Database\Seeder;
use Carbon\Carbon;

class DevicesTableSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $now = Carbon::now();
        DB::table('devices')->truncate();
        DB::table('devices')->insert([
            [
                'id' => 1,
                'user_id' => '1',
                'name' => 'Be ca',
                'identify_code' => 'be-ca-1',
                'password' => bcrypt('12344321'),
                'is_actived' => true,
                'created_at' => $now,
                'updated_at' => $now,
            ],
        ]);
    }
}
