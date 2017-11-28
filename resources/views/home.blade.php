@extends('layouts.app')

@section('content')
<div class="container" style="padding: 20px">
    <div style="display: flex; align-content: center; justify-content: center;">
        <div>
            <h2 style="font-weight: 900; text-align: center; margin: 0; margin-bottom: 20px">LED CONTROLL</h2>
        </div>
    </div>
    <div style="display: flex; align-items: center; justify-content: center;" id="control">
        <div style="align-self: flex-start;">
            <div>
                <input id="button" data-width="50" data-height="30" type="checkbox" data-toggle="toggle">
            </div>
        </div>
    </div>
</div>
@endsection

@push('before-scripts')
    <script>var jwtToken = "{!! $jwtToken !!}"</script>
@endpush
