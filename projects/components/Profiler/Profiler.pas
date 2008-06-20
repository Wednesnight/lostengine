(*
  Copyright (c) 2007, LostGenius Posse (Timo 'jomanto' Boll, Steve 'stevO' Marek)
  
  All rights reserved.
  
  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
  
      * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
      * Neither the name of the LostGenius Posse nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
  
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*)
unit Profiler;

interface

uses
  Classes,
  SysUtils;

type

  TProfilePropertyItem = class( TCollectionItem )
  protected
    FOnAddProperty : TNotifyEvent;
  public
    procedure Activate;

    property OnAddProperty : TNotifyEvent read FOnAddProperty write FOnAddProperty;
  end;

  TProfileProperties = class( TCollection )
  protected
    FProfilePath   : String;
    FOnAddProperty : TNotifyEvent;

    function GetItems( Index : Integer ) : TProfilePropertyItem;
    procedure SetItems( Index : Integer; const Value : TProfilePropertyItem );

    procedure ItemAddProperty( Sender : TObject );
  public
    procedure Activate;
    function Add : TProfilePropertyItem;

    property OnAddProperty          : TNotifyEvent         read FOnAddProperty write FOnAddProperty;
    property Items[Index : Integer] : TProfilePropertyItem read GetItems       write SetItems;
    property ProfilePath            : String               read FProfilePath   write FProfilePath;
  end;

  TProfilerItem = class( TCollectionItem )
  protected
    FOnAddProperty : TNotifyEvent;
    FProfileName   : String;
    FProfilePath   : String;
    FProperties    : TProfileProperties;

    procedure SetProfilePath( const Value : String );
    procedure PropertiesAddProperty( Sender : TObject );
  public
    constructor Create( Collection : TCollection ); override;
    destructor Destroy; override;

    procedure Activate;

    property OnAddProperty : TNotifyEvent read FOnAddProperty write FOnAddProperty;
    property ProfileName   : String       read FProfileName   write FProfileName;
    property ProfilePath   : String       read FProfilePath   write SetProfilePath;
  end;

  TProfiler = class( TCollection )
  protected
    FOnAddProperty : TNotifyEvent;
    FProfilePath   : String;

    function GetItems( Index : Integer ) : TProfilerItem;
    procedure SetItems( Index : Integer; const Value : TProfilerItem );
    procedure SetProfilePath( const Value : String );
    function GetProfiles( AName : String ) : TProfilerItem;
    procedure SetProfiles( AName : String; const Value : TProfilerItem );

    procedure ProfilerItemAddProperty( Sender : TObject );
  public
    constructor Create( AProfilePath : String = '' );

    function Add( AProfileName : String = '' ) : TProfilerItem;

    property Items[Index : Integer]   : TProfilerItem read GetItems       write SetItems;
    property OnAddProperty            : TNotifyEvent  read FOnAddProperty write FOnAddProperty;
    property ProfilePath              : String        read FProfilePath   write SetProfilePath;
    property Profiles[AName : String] : TProfilerItem read GetProfiles    write SetProfiles;
  end;

implementation

{ TProfiler }

function TProfiler.Add( AProfileName : String = '' ) : TProfilerItem;
begin
  Result:= (inherited Add as TProfilerItem);

  Result.OnAddProperty:= ProfilerItemAddProperty;
  Result.ProfilePath  := FProfilePath;
  if (Length( AProfileName ) > 0) then
    Result.ProfileName:= AProfileName;
end;

constructor TProfiler.Create( AProfilePath : String = '' );
begin
  inherited Create( TProfilerItem );

  if (Length( AProfilePath ) > 0) then
    FProfilePath:= AProfilePath;
end;

function TProfiler.GetProfiles( AName : String ) : TProfilerItem;
var
  lIndex : Integer;
begin
  Result:= nil;
  for lIndex:= 0 to Count - 1 do begin
    if (CompareStr( AName, Items[lIndex].ProfileName ) = 0) then begin
      Result:= Items[lIndex];
      Break;
    end;
  end;
end;

procedure TProfiler.SetProfiles( AName : String; const Value : TProfilerItem );
var
  lItem : TProfilerItem;
begin
  lItem:= GetProfiles( AName );
  if (lItem <> nil) then begin
    inherited Items[lItem.Index]:= Value;
  end else begin
    { TODO -ojomanto : automatically create new item if doesn't exist }
  end;
end;

procedure TProfiler.SetProfilePath( const Value : String );
var
  lIndex : Integer;
begin
  if (CompareStr( Value, FProfilePath ) <> 0) then begin
    FProfilePath:= Value;
    for lIndex:= 0 to Count - 1 do
      Items[lIndex].ProfilePath:= Value;
  end;
end;

function TProfiler.GetItems( Index : Integer ) : TProfilerItem;
begin
  Result:= (inherited Items[Index] as TProfilerItem);
end;

procedure TProfiler.SetItems( Index : Integer; const Value : TProfilerItem );
begin
  inherited Items[Index]:= Value;
end;

procedure TProfiler.ProfilerItemAddProperty( Sender : TObject );
begin
  if Assigned( FOnAddProperty ) then
    FOnAddProperty( Sender );
end;

{ TProfilerItem }

procedure TProfilerItem.Activate;
begin
  FProperties.Activate;
end;

constructor TProfilerItem.Create(Collection: TCollection);
begin
  inherited;

  FProperties              := TProfileProperties.Create( TProfilePropertyItem );
  FProperties.OnAddProperty:= PropertiesAddProperty;
end;

destructor TProfilerItem.Destroy;
begin
  FreeAndNil( FProperties );

  inherited;
end;

procedure TProfilerItem.PropertiesAddProperty( Sender : TObject );
begin
  if Assigned( FOnAddProperty ) then
    FOnAddProperty( Sender );
end;

procedure TProfilerItem.SetProfilePath( const Value : String );
begin
  FProfilePath           := Value;
  FProperties.ProfilePath:= Value;
end;

{ TProfileProperties }

procedure TProfileProperties.Activate;
var
  lIndex : Integer;
begin
  for lIndex:= 0 to Count -1 do
    Items[lIndex].Activate;
end;

function TProfileProperties.Add : TProfilePropertyItem;
begin
  Result              := (inherited Add as TProfilePropertyItem);
  Result.OnAddProperty:= ItemAddProperty;
end;

function TProfileProperties.GetItems( Index : Integer ) : TProfilePropertyItem;
begin
  Result:= (inherited Items[Index] as TProfilePropertyItem);
end;

procedure TProfileProperties.ItemAddProperty( Sender : TObject );
begin
  if Assigned( FOnAddProperty ) then
    FOnAddProperty( Sender );
end;

procedure TProfileProperties.SetItems( Index : Integer; const Value : TProfilePropertyItem );
begin
  inherited Items[Index]:= Value;
end;

{ TProfilePropertyItem }

procedure TProfilePropertyItem.Activate;
begin
  //
end;

end.
