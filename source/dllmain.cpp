#include "../../../include/IVSDK.cpp"

int   const HEALTH_MAX = 200;			// NOTE: CPed.m_fMaxHealth seems to eventually glitch and get a random value after changing the ped's health.
int   const DELAY = 15 * 1000;			// NOTE: 15 second delay.
float const INCREMENT_HEALTH = 3.f;
float healthBefore = 0;
uint64_t timeBefore = 0;
uint64_t timeSinceDamage = 0;

void HealthRegeneration()
{
	CPed* ped = CPlayerInfo::GetPlayerInfo(CWorld::PlayerInFocus)->m_pPlayerPed;
	if (!ped)
		return;

	// If player is dead |or| time hasn't passed 2s |or| player has full health
	if (ped->m_nDeathState || CTimer::m_snTimeInMilliseconds - timeBefore < 2.5 * 1000 || ped->GetHealth() >= HEALTH_MAX)
		return;

	// If the current health is less than the health before
	if (ped->GetHealth() < healthBefore) {
		timeSinceDamage = CTimer::m_snTimeInMilliseconds;
		healthBefore = ped->GetHealth();
		return;
	}

	if (CTimer::m_snTimeInMilliseconds - timeSinceDamage < DELAY)
		return;

	timeBefore = CTimer::m_snTimeInMilliseconds;

	if (ped->GetHealth() + INCREMENT_HEALTH > HEALTH_MAX)
		ped->SetHealth(HEALTH_MAX, NULL);
	else
		ped->AddHealth(INCREMENT_HEALTH);

	healthBefore = ped->GetHealth();
}

// ran after the sdk initializes, add all your hooks/events/etc here
void plugin::gameStartupEvent()
{
	plugin::processScriptsEvent::Add(HealthRegeneration);
}