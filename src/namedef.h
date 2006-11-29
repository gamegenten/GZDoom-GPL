// 'None' must always be the first name.
xx(None)

xx(Super)
xx(Object)
xx(Actor)

xx(Untranslated)

xx(Doom)
xx(Heretic)
xx(Hexen)
xx(Strife)

// blood spawning
xx(Blood)
xx(BloodSplatter)
xx(AxeBlood)
xx(Spray)

// Invulnerability types
xx(Ghost)
xx(Reflective)

// Healingradius types
xx(Mana)
xx(Armor)

// Per-actor sound channels
xx(Auto)
xx(Weapon)
xx(Voice)
xx(Item)
xx(Body)
xx(SoundSlot5)
xx(SoundSlot6)
xx(SoundSlot7)

// Hexen sound sequence names
xx(Platform)
xx(PlatformMetal)
xx(Silence)
xx(Lava)
xx(Water)
xx(Ice)
xx(Earth)
xx(PlatformMetal2)
xx(DoorNormal)
xx(DoorHeavy)
xx(DoorMetal)
xx(DoorCreak)
xx(DoorMetal2)
xx(Wind)

xx(BulletPuff)

// Special bosses A_BossDeath knows about
xx(Fatso)
xx(Arachnotron)
xx(BaronOfHell)
xx(Cyberdemon)
xx(SpiderMastermind)
xx(Ironlich)
xx(Minotaur)
xx(Sorcerer2)

// P_SpawnMapThing checks for these as health items (I smell a FIXME)
xx(Berserk)
xx(Soulsphere)
xx(Megasphere)		// also counts as armor for P_SpawnMapThing

// Standard player classes
xx(DoomPlayer)
xx(HereticPlayer)
xx(StrifePlayer)
xx(FighterPlayer)
xx(ClericPlayer)
xx(MagePlayer)
xx(ChickenPlayer)
xx(PigPlayer)

// Flechette names for the different Hexen player classes
xx(ArtiPoisonBag1)
xx(ArtiPoisonBag2)
xx(ArtiPoisonBag3)

// Strife quests
xx(QuestItem)

// Auto-usable health items
xx(ArtiHealth)
xx(ArtiSuperHealth)
xx(MedicalKit)
xx(MedPatch)

// The Wings of Wrath
xx(ArtiFly)

// Doom ammo types
xx(Clip)
xx(Shell)
xx(RocketAmmo)
xx(Cell)

// Hexen Mana
xx(Mana1)
xx(Mana2)

// Ammo and weapon names for the Strife status bar
xx(ClipOfBullets)
xx(PoisonBolts)
xx(ElectricBolts)
xx(HEGrenadeRounds)
xx(PhosphorusGrenadeRounds)
xx(MiniMissiles)
xx(EnergyPod)

xx(StrifeCrossbow)
xx(AssaultGun)
xx(FlameThrower)
xx(MiniMissileLauncher)
xx(StrifeGrenadeLauncher)
xx(Mauler)

xx(Chicken)
xx(Pig)

// Standard animator names.
xx(Spawn)
xx(See)
xx(Pain)
xx(Melee)
xx(Missile)
xx(Crash)
xx(Death)
xx(Raise)
xx(Wound)
xx(Heal)
xx(Crush)
xx(Yes)
xx(No)
xx(Greetings)

// Compatible death names for the decorate parser.
xx(XDeath)
xx(Burn)
//xx(Ice)			// already defined above
xx(Disintegrate)

// Weapon animator names.
xx(Select)
xx(Deselect)
xx(Ready)
xx(Fire)
xx(Hold)
xx(AltFire)
xx(AltHold)
xx(Flash)
xx(AltFlash)

// State names used by ASwitchableDecoration
xx(Active)
xx(Inactive)

// State names used by ACustomInventory
xx(Pickup)
xx(Use)
xx(Drop)

// Damage types
//xx(Fire)		already defined above
//xx(Ice)
//xx(Disintegrate)
//xx(Water)
xx(Slime)
//xx(Crush)
xx(Telefrag)
xx(Falling)
xx(Suicide)
xx(Exit)
xx(Railgun)
xx(Poison)
xx(Electric)
xx(BFGSplash)
xx(DrainLife)	// A weapon like the Sigil that drains your life away.
xx(Massacre)	// For death by a cheater!
//(Melee)		already defined above, so don't define it again

// Special death name for getting killed excessively. Could be used as
// a damage type if you wanted to force an extreme death.
xx(Extreme)

// Special names for thingdef_exp.cpp
xx(Random)
xx(Random2)
xx(Cos)
xx(Sin)
xx(Alpha)
xx(Angle)
xx(Args)
xx(CeilingZ)
xx(FloorZ)
xx(Health)
xx(Pitch)
xx(Special)
xx(TID)
xx(TIDtoHate)
xx(WaterLevel)
xx(X)
xx(Y)
xx(Z)
