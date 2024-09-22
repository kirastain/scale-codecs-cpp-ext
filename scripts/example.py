from substrateinterface import Keypair, SubstrateInterface
from substrateinterface.storage import StorageKey
from substrateinterface.exceptions import SubstrateRequestException
from scalecodec.base import ScaleBytes, RuntimeConfigurationObject, ScaleType
# WebSocket connection to Vara Testnet.
ws = SubstrateInterface(url="wss://testnet.vara.network")

# Creation of `pallet_gear::send_message(..)` extrinsic/call.
call = ws.compose_call(
    call_module='Gear',
    call_function='send_message',
    call_params={
        # Predefined Ping-Pong program.
        'destination': '0xc1e4c656819d61ecac43cccbbad7320fc350a438e862b13ce19285536b9fa99b',
        # Hex repr of "PING".
        'payload': '0x50494e47',
        # Random gas limit that will cover calculations.
        'gas_limit': 10**9,
        # Zero value applied to message.
        'value': 0,
        # Keeping sender account alive on balance below existential deposit.
        'keep_alive': True,
    }
)

# Storage key for chain events.
key = StorageKey.create_from_storage_function(
    pallet="System",
    storage_function="Events",
    params=[],
    runtime_config=ws.runtime_config,
    metadata=ws.metadata
)

# The raw data from chain
data = "0x080000000000000082c9b343551702000000010000000000c244223d00020100"

updated_obj = ws.runtime_config.create_scale_object(
                type_string=key.value_scale_type,
                data=ScaleBytes(data),
                metadata=ws.metadata
            )
updated_obj.decode()

# Decoded data
print(updated_obj)
"""
[{'phase': 'ApplyExtrinsic', 'extrinsic_idx': 0, 'event': {'event_index': '0000', 'module_id': 'System', 'event_id': 'ExtrinsicSuccess', 'attributes': {'dispatch_info': {'weight': {'ref_time': 283964000, 'proof_size': 1493}, 'class': 'Mandatory', 'pays_fee': 'Yes'}}}, 'event_index': 0, 'module_id': 'System', 'event_id': 'ExtrinsicSuccess', 'attributes': {'dispatch_info': {'weight': {'ref_time': 283964000, 'proof_size': 1493}, 'class': 'Mandatory', 'pays_fee': 'Yes'}}, 'topics': []}, {'phase': 'ApplyExtrinsic', 'extrinsic_idx': 1, 'event': {'event_index': '0000', 'module_id': 'System', 'event_id': 'ExtrinsicSuccess', 'attributes': {'dispatch_info': {'weight': {'ref_time': 256414000, 'proof_size': 0}, 'class': 'Mandatory', 'pays_fee': 'No'}}}, 'event_index': 0, 'module_id': 'System', 'event_id': 'ExtrinsicSuccess', 'attributes': {'dispatch_info': {'weight': {'ref_time': 256414000, 'proof_size': 0}, 'class': 'Mandatory', 'pays_fee': 'No'}}, 'topics': []}]
"""
